#ifndef MECHANO_HARDWARE__ARDUINO_COMMS_HPP_
#define MECHANO_HARDWARE__ARDUINO_COMMS_HPP_

#include <string>

namespace mechano_hardware
{

class ArduinoComms
{
public:
  ArduinoComms() = default;

  void setup(const std::string & device, int baud_rate, int timeout_ms);

  bool connect();
  void disconnect();
  bool is_connected() const;

  bool read_encoder_values(int & left_enc, int & right_enc);
  void set_motor_values(double left_vel, double right_vel);

private:
  std::string device_;
  int baud_rate_;
  int timeout_ms_;
  int fd_ = -1;
  bool connected_ = false;

  void send_command(const std::string & cmd);
  std::string read_response();
};

}  // namespace mechano_hardware

#endif  // MECHANO_HARDWARE__ARDUINO_COMMS_HPP_
