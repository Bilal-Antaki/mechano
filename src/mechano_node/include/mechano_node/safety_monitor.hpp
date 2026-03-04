#ifndef MECHANO_NODE__SAFETY_MONITOR_HPP_
#define MECHANO_NODE__SAFETY_MONITOR_HPP_

#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/range.hpp"

namespace mechano_node
{

class SafetyMonitor
{
public:
  using EStopCallback = std::function<void(bool activate)>;

  explicit SafetyMonitor(rclcpp::Node * node);

  void set_estop_callback(EStopCallback callback);
  bool is_obstacle_detected() const;
  void update();

private:
  void range_callback(const sensor_msgs::msg::Range::SharedPtr msg);

  rclcpp::Node * node_;
  rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr range_subscription_;

  EStopCallback estop_callback_;

  float obstacle_distance_;
  float min_distance_threshold_;
  bool obstacle_detected_;
};

}  // namespace mechano_node

#endif  // MECHANO_NODE__SAFETY_MONITOR_HPP_
