#ifndef MECHANO_NODE__BATTERY_MONITOR_HPP_
#define MECHANO_NODE__BATTERY_MONITOR_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/battery_state.hpp"

namespace mechano_node
{

class BatteryMonitor
{
public:
  explicit BatteryMonitor(rclcpp::Node * node);

  float get_percentage() const;
  float get_voltage() const;
  bool is_low() const;
  bool is_critical() const;

  void update();

private:
  void battery_callback(const sensor_msgs::msg::BatteryState::SharedPtr msg);

  rclcpp::Node * node_;
  rclcpp::Subscription<sensor_msgs::msg::BatteryState>::SharedPtr subscription_;

  float voltage_;
  float percentage_;
  float low_threshold_;
  float critical_threshold_;
};

}  // namespace mechano_node

#endif  // MECHANO_NODE__BATTERY_MONITOR_HPP_
