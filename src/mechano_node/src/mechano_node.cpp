#include "mechano_node/mechano_node.hpp"

#include <chrono>
#include <functional>

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;

namespace mechano_node
{

MechanoNode::MechanoNode(const rclcpp::NodeOptions & options)
: Node("mechano_node", options),
  current_mode_(mechano_msgs::msg::RobotState::MODE_IDLE),
  estop_active_(false)
{
  // Create publisher
  state_publisher_ = this->create_publisher<mechano_msgs::msg::RobotState>("robot_state", 10);

  // Create services
  set_mode_service_ = this->create_service<mechano_msgs::srv::SetMode>(
    "set_mode",
    std::bind(&MechanoNode::set_mode_callback, this, _1, _2));

  estop_service_ = this->create_service<mechano_msgs::srv::EStop>(
    "estop",
    std::bind(&MechanoNode::estop_callback, this, _1, _2));

  // Create timer for publishing state
  timer_ = this->create_wall_timer(
    100ms,
    std::bind(&MechanoNode::timer_callback, this));

  // Initialize components
  hmi_ = std::make_unique<HMI>(this);
  battery_monitor_ = std::make_unique<BatteryMonitor>(this);
  safety_monitor_ = std::make_unique<SafetyMonitor>(this);

  // Set up safety monitor callback
  safety_monitor_->set_estop_callback([this](bool activate) {
      if (activate && !estop_active_) {
        RCLCPP_WARN(this->get_logger(), "Safety E-Stop triggered!");
        estop_active_ = true;
        hmi_->set_led(LedColor::RED, LedPattern::BLINK_FAST);
      }
    });

  // Set up HMI button callback
  hmi_->set_button_callback([this](int button_id) {
      RCLCPP_INFO(this->get_logger(), "Button %d pressed", button_id);
      // Handle button presses here
    });

  // Initial LED state
  hmi_->set_led(LedColor::GREEN, LedPattern::SOLID);

  RCLCPP_INFO(this->get_logger(), "Mechano node started");
}

void MechanoNode::timer_callback()
{
  // Update components
  hmi_->update();
  battery_monitor_->update();
  safety_monitor_->update();

  // Publish robot state
  auto msg = mechano_msgs::msg::RobotState();
  msg.header.stamp = this->now();
  msg.mode = current_mode_;
  msg.battery_pct = battery_monitor_->get_percentage();
  msg.estop_active = estop_active_;
  msg.error_code = 0;
  msg.error_message = "";

  // Battery alerts — only update LED if E-Stop is not active (E-Stop LED takes priority)
  if (battery_monitor_->is_critical()) {
    msg.error_code = 1;
    msg.error_message = "Critical battery level";
    if (!estop_active_) {
      hmi_->set_led(LedColor::RED, LedPattern::BLINK_FAST);
    }
  } else if (battery_monitor_->is_low() && !estop_active_) {
    hmi_->set_led(LedColor::YELLOW, LedPattern::BLINK_SLOW);
  }

  state_publisher_->publish(msg);
}

void MechanoNode::set_mode_callback(
  const std::shared_ptr<mechano_msgs::srv::SetMode::Request> request,
  std::shared_ptr<mechano_msgs::srv::SetMode::Response> response)
{
  if (estop_active_ && request->mode != mechano_msgs::msg::RobotState::MODE_IDLE) {
    response->success = false;
    response->message = "Cannot change mode while E-Stop is active";
    return;
  }

  current_mode_ = request->mode;

  switch (current_mode_) {
    case mechano_msgs::msg::RobotState::MODE_IDLE:
      hmi_->set_led(LedColor::GREEN, LedPattern::SOLID);
      break;
    case mechano_msgs::msg::RobotState::MODE_TELEOP:
      hmi_->set_led(LedColor::BLUE, LedPattern::SOLID);
      break;
    case mechano_msgs::msg::RobotState::MODE_AUTONOMOUS:
      hmi_->set_led(LedColor::GREEN, LedPattern::PULSE);
      break;
    default:
      break;
  }

  response->success = true;
  response->message = "Mode changed successfully";

  RCLCPP_INFO(this->get_logger(), "Mode changed to %d", current_mode_);
}

void MechanoNode::estop_callback(
  const std::shared_ptr<mechano_msgs::srv::EStop::Request> request,
  std::shared_ptr<mechano_msgs::srv::EStop::Response> response)
{
  estop_active_ = request->activate;

  if (estop_active_) {
    current_mode_ = mechano_msgs::msg::RobotState::MODE_IDLE;
    hmi_->set_led(LedColor::RED, LedPattern::SOLID);
    RCLCPP_WARN(this->get_logger(), "E-Stop activated");
  } else {
    hmi_->set_led(LedColor::GREEN, LedPattern::SOLID);
    RCLCPP_INFO(this->get_logger(), "E-Stop released");
  }

  response->success = true;
  response->message = estop_active_ ? "E-Stop activated" : "E-Stop released";
}

}  // namespace mechano_node

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<mechano_node::MechanoNode>());
  rclcpp::shutdown();
  return 0;
}
