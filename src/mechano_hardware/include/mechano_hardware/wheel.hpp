#ifndef MECHANO_HARDWARE__WHEEL_HPP_
#define MECHANO_HARDWARE__WHEEL_HPP_

#include <string>
#include <cmath>

namespace mechano_hardware
{

class Wheel
{
public:
  std::string name = "";
  int enc_counts_per_rev = 0;

  // State
  double pos = 0.0;  // radians
  double vel = 0.0;  // rad/s

  // Command
  double cmd = 0.0;  // rad/s

  // Encoder tracking
  int enc = 0;

  Wheel() = default;

  Wheel(const std::string & wheel_name, int counts_per_rev)
  : name(wheel_name), enc_counts_per_rev(counts_per_rev)
  {
  }

  void setup(const std::string & wheel_name, int counts_per_rev)
  {
    name = wheel_name;
    enc_counts_per_rev = counts_per_rev;
  }

  double calc_enc_angle() const
  {
    if (enc_counts_per_rev == 0) {
      return 0.0;
    }
    return static_cast<double>(enc) / enc_counts_per_rev * 2.0 * M_PI;
  }
};

}  // namespace mechano_hardware

#endif  // MECHANO_HARDWARE__WHEEL_HPP_
