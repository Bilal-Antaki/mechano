#include "mechano_hardware/mechano_system.hpp"

#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"

namespace mechano_hardware
{

hardware_interface::CallbackReturn MechanoSystemHardware::on_init(
  const hardware_interface::HardwareInfo & info)
{
  if (hardware_interface::SystemInterface::on_init(info) !=
    hardware_interface::CallbackReturn::SUCCESS)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  // Get parameters from URDF — use .at() and catch so missing params return ERROR, not throw
  try {
    device_ = info_.hardware_parameters.at("device");
    baud_rate_ = std::stoi(info_.hardware_parameters.at("baud_rate"));
    timeout_ms_ = std::stoi(info_.hardware_parameters.at("timeout_ms"));
    enc_counts_per_rev_ = std::stoi(info_.hardware_parameters.at("enc_counts_per_rev"));
  } catch (const std::exception & e) {
    RCLCPP_ERROR(
      rclcpp::get_logger("MechanoSystemHardware"),
      "Missing or invalid hardware parameter: %s", e.what());
    return hardware_interface::CallbackReturn::ERROR;
  }

  // Find wheels by name to avoid assuming joint ordering in the URDF
  for (const auto & joint : info_.joints) {
    if (joint.name.find("left") != std::string::npos) {
      left_wheel_.setup(joint.name, enc_counts_per_rev_);
    } else if (joint.name.find("right") != std::string::npos) {
      right_wheel_.setup(joint.name, enc_counts_per_rev_);
    }
  }
  if (left_wheel_.name.empty() || right_wheel_.name.empty()) {
    RCLCPP_ERROR(
      rclcpp::get_logger("MechanoSystemHardware"),
      "Could not find left and right wheel joints in URDF");
    return hardware_interface::CallbackReturn::ERROR;
  }

  RCLCPP_INFO(
    rclcpp::get_logger("MechanoSystemHardware"),
    "Successfully initialized with device: %s", device_.c_str());

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MechanoSystemHardware::on_configure(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Configuring...");

  comms_.setup(device_, baud_rate_, timeout_ms_);

  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Configuration complete");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MechanoSystemHardware::on_cleanup(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Cleaning up...");

  if (comms_.is_connected()) {
    comms_.disconnect();
  }

  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Cleanup complete");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MechanoSystemHardware::on_activate(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Activating...");

  if (!comms_.connect()) {
    RCLCPP_ERROR(
      rclcpp::get_logger("MechanoSystemHardware"),
      "Failed to connect to hardware at %s", device_.c_str());
    return hardware_interface::CallbackReturn::ERROR;
  }

  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Activation complete");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn MechanoSystemHardware::on_deactivate(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Deactivating...");

  // Stop motors
  comms_.set_motor_values(0.0, 0.0);
  comms_.disconnect();

  RCLCPP_INFO(rclcpp::get_logger("MechanoSystemHardware"), "Deactivation complete");
  return hardware_interface::CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
MechanoSystemHardware::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;

  state_interfaces.emplace_back(
    hardware_interface::StateInterface(
      left_wheel_.name, hardware_interface::HW_IF_POSITION, &left_wheel_.pos));
  state_interfaces.emplace_back(
    hardware_interface::StateInterface(
      left_wheel_.name, hardware_interface::HW_IF_VELOCITY, &left_wheel_.vel));

  state_interfaces.emplace_back(
    hardware_interface::StateInterface(
      right_wheel_.name, hardware_interface::HW_IF_POSITION, &right_wheel_.pos));
  state_interfaces.emplace_back(
    hardware_interface::StateInterface(
      right_wheel_.name, hardware_interface::HW_IF_VELOCITY, &right_wheel_.vel));

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface>
MechanoSystemHardware::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;

  command_interfaces.emplace_back(
    hardware_interface::CommandInterface(
      left_wheel_.name, hardware_interface::HW_IF_VELOCITY, &left_wheel_.cmd));

  command_interfaces.emplace_back(
    hardware_interface::CommandInterface(
      right_wheel_.name, hardware_interface::HW_IF_VELOCITY, &right_wheel_.cmd));

  return command_interfaces;
}

hardware_interface::return_type MechanoSystemHardware::read(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & period)
{
  if (!comms_.is_connected()) {
    return hardware_interface::return_type::ERROR;
  }

  // Start from previous encoder values; read_encoder_values only updates them on success
  int left_enc = left_wheel_.enc;
  int right_enc = right_wheel_.enc;

  if (!comms_.read_encoder_values(left_enc, right_enc)) {
    RCLCPP_WARN(
      rclcpp::get_logger("MechanoSystemHardware"), "Encoder read failed, keeping previous values");
    return hardware_interface::return_type::OK;
  }

  double delta_seconds = period.seconds();
  if (delta_seconds > 0) {
    int left_delta = left_enc - left_wheel_.enc;
    int right_delta = right_enc - right_wheel_.enc;

    left_wheel_.vel = (left_delta / static_cast<double>(enc_counts_per_rev_) * 2.0 * M_PI) /
      delta_seconds;
    right_wheel_.vel = (right_delta / static_cast<double>(enc_counts_per_rev_) * 2.0 * M_PI) /
      delta_seconds;
  }

  left_wheel_.enc = left_enc;
  right_wheel_.enc = right_enc;
  left_wheel_.pos = left_wheel_.calc_enc_angle();
  right_wheel_.pos = right_wheel_.calc_enc_angle();

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type MechanoSystemHardware::write(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
{
  if (!comms_.is_connected()) {
    return hardware_interface::return_type::ERROR;
  }

  // Send velocity commands to motors
  comms_.set_motor_values(left_wheel_.cmd, right_wheel_.cmd);

  return hardware_interface::return_type::OK;
}

}  // namespace mechano_hardware

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
  mechano_hardware::MechanoSystemHardware,
  hardware_interface::SystemInterface)
