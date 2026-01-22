#include "mechano_perception/distance_sensor_node.hpp"

#include <chrono>
#include <functional>

using namespace std::chrono_literals;

namespace mechano_perception
{

DistanceSensorNode::DistanceSensorNode(const rclcpp::NodeOptions & options)
: Node("distance_sensor_node", options),
  fd_(-1),
  connected_(false)
{
  // Declare parameters
  this->declare_parameter("frame_id", "distance_sensor_link");
  this->declare_parameter("device", "/dev/ttyUSB1");
  this->declare_parameter("min_range", 0.02);
  this->declare_parameter("max_range", 2.0);
  this->declare_parameter("field_of_view", 0.1);
  this->declare_parameter("publish_rate", 10);

  // Get parameters
  frame_id_ = this->get_parameter("frame_id").as_string();
  device_ = this->get_parameter("device").as_string();
  min_range_ = this->get_parameter("min_range").as_double();
  max_range_ = this->get_parameter("max_range").as_double();
  field_of_view_ = this->get_parameter("field_of_view").as_double();
  publish_rate_ = this->get_parameter("publish_rate").as_int();

  // Create publisher
  publisher_ = this->create_publisher<sensor_msgs::msg::Range>("distance", 10);

  // Create timer
  auto period = std::chrono::milliseconds(1000 / publish_rate_);
  timer_ = this->create_wall_timer(period, std::bind(&DistanceSensorNode::timer_callback, this));

  RCLCPP_INFO(this->get_logger(), "Distance sensor node started");
  RCLCPP_INFO(this->get_logger(), "  Device: %s", device_.c_str());
  RCLCPP_INFO(this->get_logger(), "  Frame: %s", frame_id_.c_str());
  RCLCPP_INFO(this->get_logger(), "  Range: %.2f - %.2f m", min_range_, max_range_);
}

void DistanceSensorNode::timer_callback()
{
  auto msg = sensor_msgs::msg::Range();

  msg.header.stamp = this->now();
  msg.header.frame_id = frame_id_;

  msg.radiation_type = sensor_msgs::msg::Range::INFRARED;
  msg.field_of_view = field_of_view_;
  msg.min_range = min_range_;
  msg.max_range = max_range_;

  // Read distance from sensor
  msg.range = read_distance();

  publisher_->publish(msg);
}

double DistanceSensorNode::read_distance()
{
  // Placeholder implementation
  // In a real implementation, this would read from the actual sensor
  // For example, VL53L0X via I2C or serial

  // Return a dummy value for now
  // This should be replaced with actual sensor reading code
  static double dummy_distance = 1.0;

  // Simulate some variation
  dummy_distance += 0.01;
  if (dummy_distance > max_range_) {
    dummy_distance = min_range_;
  }

  return dummy_distance;
}

}  // namespace mechano_perception

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<mechano_perception::DistanceSensorNode>());
  rclcpp::shutdown();
  return 0;
}
