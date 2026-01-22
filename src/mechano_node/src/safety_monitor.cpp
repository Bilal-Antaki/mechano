#include "mechano_node/safety_monitor.hpp"

namespace mechano_node
{

SafetyMonitor::SafetyMonitor(rclcpp::Node * node)
: node_(node),
  obstacle_distance_(std::numeric_limits<float>::max()),
  min_distance_threshold_(0.2),
  obstacle_detected_(false)
{
  // Declare parameters
  node_->declare_parameter("safety.min_distance", 0.2);
  min_distance_threshold_ = node_->get_parameter("safety.min_distance").as_double();

  // Subscribe to range sensor
  range_subscription_ = node_->create_subscription<sensor_msgs::msg::Range>(
    "distance",
    10,
    std::bind(&SafetyMonitor::range_callback, this, std::placeholders::_1));

  RCLCPP_DEBUG(node_->get_logger(), "Safety monitor initialized");
}

void SafetyMonitor::set_estop_callback(EStopCallback callback)
{
  estop_callback_ = callback;
}

void SafetyMonitor::range_callback(const sensor_msgs::msg::Range::SharedPtr msg)
{
  obstacle_distance_ = msg->range;

  bool was_detected = obstacle_detected_;
  obstacle_detected_ = (obstacle_distance_ < min_distance_threshold_);

  // Trigger E-Stop if obstacle suddenly detected
  if (obstacle_detected_ && !was_detected && estop_callback_) {
    RCLCPP_WARN(
      node_->get_logger(),
      "Obstacle detected at %.2f m (threshold: %.2f m)",
      obstacle_distance_,
      min_distance_threshold_);
    estop_callback_(true);
  }
}

void SafetyMonitor::cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  (void)msg;  // Unused for now
  // Could be used to monitor velocity for safety checks
}

bool SafetyMonitor::is_obstacle_detected() const
{
  return obstacle_detected_;
}

void SafetyMonitor::update()
{
  // Additional safety checks could go here
  // For example, checking for stalled motors, tipping, etc.
}

}  // namespace mechano_node
