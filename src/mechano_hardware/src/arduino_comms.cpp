#include "mechano_hardware/arduino_comms.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
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

  // Read timeout
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = timeout_ms_ / 100;

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
  write(fd_, full_cmd.c_str(), full_cmd.length());
}

std::string ArduinoComms::read_response()
{
  if (!connected_) {
    return "";
  }

  char buf[256];
  std::string response;

  while (true) {
    int n = read(fd_, buf, sizeof(buf) - 1);
    if (n <= 0) {
      break;
    }
    buf[n] = '\0';
    response += buf;

    if (response.find('\n') != std::string::npos) {
      break;
    }
  }

  // Remove trailing newline
  if (!response.empty() && response.back() == '\n') {
    response.pop_back();
  }
  if (!response.empty() && response.back() == '\r') {
    response.pop_back();
  }

  return response;
}

void ArduinoComms::read_encoder_values(int & left_enc, int & right_enc)
{
  // Send encoder request
  send_command("e");

  // Read response
  std::string response = read_response();

  // Parse response: "left_enc right_enc"
  std::istringstream iss(response);
  if (!(iss >> left_enc >> right_enc)) {
    // Parse failed, keep previous values
    left_enc = 0;
    right_enc = 0;
  }
}

void ArduinoComms::set_motor_values(double left_vel, double right_vel)
{
  // Send velocity command: "v left_vel right_vel"
  std::ostringstream oss;
  oss << "v " << left_vel << " " << right_vel;
  send_command(oss.str());
}

}  // namespace mechano_hardware
