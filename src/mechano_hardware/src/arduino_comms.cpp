#include "mechano_hardware/arduino_comms.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>

namespace mechano_hardware
{

void ArduinoComms::setup(const std::string & device, int baud_rate, int timeout_ms)
{
  device_ = device;
  baud_rate_ = baud_rate;
  timeout_ms_ = timeout_ms;
}

bool ArduinoComms::connect()
{
  if (connected_) {
    return true;
  }

  // Open serial port
  fd_ = open(device_.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (fd_ < 0) {
    return false;
  }

  // Configure serial port
  struct termios tty;
  if (tcgetattr(fd_, &tty) != 0) {
    close(fd_);
    fd_ = -1;
    return false;
  }

  // Set baud rate
  speed_t baud;
  switch (baud_rate_) {
    case 9600: baud = B9600; break;
    case 19200: baud = B19200; break;
    case 38400: baud = B38400; break;
    case 57600: baud = B57600; break;
    case 115200: baud = B115200; break;
    default: baud = B115200;
  }
  cfsetospeed(&tty, baud);
  cfsetispeed(&tty, baud);

  // Configure 8N1
  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
  tty.c_cflag &= ~(PARENB | PARODD);
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag |= (CLOCAL | CREAD);

  // Raw input
  tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

  // Raw output
  tty.c_oflag &= ~OPOST;

  // VTIME is in tenths of a second; clamp to at least 1 to avoid non-blocking mode
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = static_cast<cc_t>(std::max(1, timeout_ms_ / 100));

  if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
    close(fd_);
    fd_ = -1;
    return false;
  }

  connected_ = true;
  return true;
}

void ArduinoComms::disconnect()
{
  if (fd_ >= 0) {
    close(fd_);
    fd_ = -1;
  }
  connected_ = false;
}

bool ArduinoComms::is_connected() const
{
  return connected_;
}

void ArduinoComms::send_command(const std::string & cmd)
{
  if (!connected_) {
    return;
  }

  std::string full_cmd = cmd + "\n";
  ssize_t written = write(fd_, full_cmd.c_str(), full_cmd.size());
  if (written < 0 || static_cast<size_t>(written) != full_cmd.size()) {
    close(fd_);
    fd_ = -1;
    connected_ = false;
  }
}

std::string ArduinoComms::read_response()
{
  if (!connected_) {
    return "";
  }

  char buf[256];
  std::string response;
  response.reserve(64);

  while (true) {
    int n = read(fd_, buf, sizeof(buf) - 1);
    if (n <= 0) {
      break;
    }
    response.append(buf, n);

    if (response.find('\n') != std::string::npos) {
      break;
    }
  }

  // Strip trailing CR/LF
  while (!response.empty() &&
    (response.back() == '\n' || response.back() == '\r'))
  {
    response.pop_back();
  }

  return response;
}

bool ArduinoComms::read_encoder_values(int & left_enc, int & right_enc)
{
  send_command("e");

  std::istringstream iss(read_response());
  if (!(iss >> left_enc >> right_enc)) {
    return false;  // caller keeps previous values
  }
  return true;
}

void ArduinoComms::set_motor_values(double left_vel, double right_vel)
{
  char buf[64];
  std::snprintf(buf, sizeof(buf), "v %.6f %.6f", left_vel, right_vel);
  send_command(buf);
}

}  // namespace mechano_hardware
