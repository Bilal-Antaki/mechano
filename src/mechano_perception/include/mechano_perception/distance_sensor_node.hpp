#ifndef MECHANO_PERCEPTION__DISTANCE_SENSOR_NODE_HPP_
#define MECHANO_PERCEPTION__DISTANCE_SENSOR_NODE_HPP_

#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/range.hpp"

namespace mechano_perception
{

class DistanceSensorNode : public rclcpp::Node
{
public:
  explicit DistanceSensorNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
  void timer_callback();

  rclcpp::Publisher<sensor_msgs::msg::Range>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  // Parameters
  std::string frame_id_;
  std::string device_;
  double min_range_;
  double max_range_;
  double field_of_view_;
  int publish_rate_;

  // Serial port handle (placeholder for actual implementation)
  int fd_;
  bool connected_;

  double read_distance();
};

}  // namespace mechano_perception

#endif  // MECHANO_PERCEPTION__DISTANCE_SENSOR_NODE_HPP_
