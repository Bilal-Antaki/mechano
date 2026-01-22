#ifndef MECHANO_NODE__HMI_HPP_
#define MECHANO_NODE__HMI_HPP_

#include <string>
#include <functional>

#include "rclcpp/rclcpp.hpp"

namespace mechano_node
{

enum class LedPattern
{
  OFF,
  SOLID,
  BLINK_SLOW,
  BLINK_FAST,
  PULSE
};

enum class LedColor
{
  GREEN,
  YELLOW,
  RED,
  BLUE
};

class HMI
{
public:
  using ButtonCallback = std::function<void(int button_id)>;

  explicit HMI(rclcpp::Node * node);

  void set_led(LedColor color, LedPattern pattern);
  void set_button_callback(ButtonCallback callback);
  void update();

private:
  rclcpp::Node * node_;
  ButtonCallback button_callback_;

  LedColor current_color_;
  LedPattern current_pattern_;

  // GPIO or serial interface would go here
};

}  // namespace mechano_node

#endif  // MECHANO_NODE__HMI_HPP_
