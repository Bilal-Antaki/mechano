#include "mechano_node/hmi.hpp"

namespace mechano_node
{

HMI::HMI(rclcpp::Node * node)
: node_(node),
  current_color_(LedColor::GREEN),
  current_pattern_(LedPattern::OFF)
{
  RCLCPP_DEBUG(node_->get_logger(), "HMI initialized");
}

void HMI::set_led(LedColor color, LedPattern pattern)
{
  current_color_ = color;
  current_pattern_ = pattern;

  // In a real implementation, this would write to GPIO or send commands
  // to an LED controller via serial/I2C
  RCLCPP_DEBUG(
    node_->get_logger(),
    "LED set to color %d, pattern %d",
    static_cast<int>(color),
    static_cast<int>(pattern));
}

void HMI::set_button_callback(ButtonCallback callback)
{
  button_callback_ = callback;
}

void HMI::update()
{
  // In a real implementation, this would:
  // 1. Read button states from GPIO
  // 2. Call button_callback_ when buttons are pressed
  // 3. Update LED patterns (for blinking/pulsing)

  // Placeholder for GPIO reading
  // if (button_pressed && button_callback_) {
  //   button_callback_(button_id);
  // }
}

}  // namespace mechano_node
