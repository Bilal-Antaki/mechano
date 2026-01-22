#ifndef MECHANO_NODE__MECHANO_NODE_HPP_
#define MECHANO_NODE__MECHANO_NODE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "mechano_msgs/msg/robot_state.hpp"
#include "mechano_msgs/srv/set_mode.hpp"
#include "mechano_msgs/srv/e_stop.hpp"

#include "mechano_node/hmi.hpp"
#include "mechano_node/battery_monitor.hpp"
#include "mechano_node/safety_monitor.hpp"

namespace mechano_node
{

class MechanoNode : public rclcpp::Node
{
public:
  explicit MechanoNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
  void timer_callback();
  void set_mode_callback(
    const std::shared_ptr<mechano_msgs::srv::SetMode::Request> request,
    std::shared_ptr<mechano_msgs::srv::SetMode::Response> response);
  void estop_callback(
    const std::shared_ptr<mechano_msgs::srv::EStop::Request> request,
    std::shared_ptr<mechano_msgs::srv::EStop::Response> response);

  // Publishers
  rclcpp::Publisher<mechano_msgs::msg::RobotState>::SharedPtr state_publisher_;

  // Services
  rclcpp::Service<mechano_msgs::srv::SetMode>::SharedPtr set_mode_service_;
  rclcpp::Service<mechano_msgs::srv::EStop>::SharedPtr estop_service_;

  // Timer
  rclcpp::TimerBase::SharedPtr timer_;

  // Components
  std::unique_ptr<HMI> hmi_;
  std::unique_ptr<BatteryMonitor> battery_monitor_;
  std::unique_ptr<SafetyMonitor> safety_monitor_;

  // State
  uint8_t current_mode_;
  bool estop_active_;
};

}  // namespace mechano_node

#endif  // MECHANO_NODE__MECHANO_NODE_HPP_
