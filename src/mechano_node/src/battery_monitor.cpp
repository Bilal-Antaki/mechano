#include "mechano_node/battery_monitor.hpp"

namespace mechano_node
{

BatteryMonitor::BatteryMonitor(rclcpp::Node * node)
: node_(node),
  voltage_(12.0),
  percentage_(100.0),
  low_threshold_(20.0),
  critical_threshold_(10.0)
{
  // Declare parameters
  node_->declare_parameter("battery.low_threshold", 20.0);
  node_->declare_parameter("battery.critical_threshold", 10.0);

  low_threshold_ = node_->get_parameter("battery.low_threshold").as_double();
  critical_threshold_ = node_->get_parameter("battery.critical_threshold").as_double();

  // Subscribe to battery state
  subscription_ = node_->create_subscription<sensor_msgs::msg::BatteryState>(
    "battery_state",
    10,
    std::bind(&BatteryMonitor::battery_callback, this, std::placeholders::_1));

  RCLCPP_DEBUG(node_->get_logger(), "Battery monitor initialized");
}

void BatteryMonitor::battery_callback(const sensor_msgs::msg::BatteryState::SharedPtr msg)
{
  voltage_ = msg->voltage;
  percentage_ = msg->percentage * 100.0;  // Convert to percentage
}

float BatteryMonitor::get_percentage() const
{
  return percentage_;
}

float BatteryMonitor::get_voltage() const
{
  return voltage_;
}

bool BatteryMonitor::is_low() const
{
  return percentage_ < low_threshold_;
}

bool BatteryMonitor::is_critical() const
{
  return percentage_ < critical_threshold_;
}

void BatteryMonitor::update()
{
  // In a real implementation, this might read directly from ADC
  // or update cached values
}

}  // namespace mechano_node
