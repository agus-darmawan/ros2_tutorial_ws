// params_node.cpp
// Demonstrates declaring, reading, and reacting to parameter changes.

#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class ParamsNode : public rclcpp::Node
{
public:
  ParamsNode() : Node("params_node")
  {
    // 1) Declare parameters with default values.
    //    Anything not declared first cannot be set later (since Foxy).
    this->declare_parameter<std::string>("robot_name", "turtle");
    this->declare_parameter<int>("max_speed", 10);
    this->declare_parameter<double>("loop_rate_hz", 1.0);
    this->declare_parameter<std::vector<std::string>>(
      "waypoints", std::vector<std::string>{"A", "B", "C"});

    // 2) Read parameters
    auto robot_name = this->get_parameter("robot_name").as_string();
    auto max_speed = this->get_parameter("max_speed").as_int();
    auto rate = this->get_parameter("loop_rate_hz").as_double();
    auto waypoints = this->get_parameter("waypoints").as_string_array();

    RCLCPP_INFO(get_logger(),
      "robot_name=%s, max_speed=%ld, loop_rate_hz=%.2f",
      robot_name.c_str(), max_speed, rate);

    for (const auto & wp : waypoints) {
      RCLCPP_INFO(get_logger(), "  waypoint: %s", wp.c_str());
    }

    // 3) React to parameter changes at runtime
    param_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&ParamsNode::on_param_change, this, std::placeholders::_1));

    // Timer to show current value periodically
    timer_ = create_wall_timer(2s, [this]() {
      RCLCPP_INFO(get_logger(), "Current max_speed = %ld",
        this->get_parameter("max_speed").as_int());
    });
  }

private:
  rcl_interfaces::msg::SetParametersResult on_param_change(
    const std::vector<rclcpp::Parameter> & params)
  {
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;

    for (const auto & p : params) {
      if (p.get_name() == "max_speed") {
        if (p.as_int() < 0 || p.as_int() > 100) {
          result.successful = false;
          result.reason = "max_speed must be in [0, 100]";
          RCLCPP_WARN(get_logger(), "Rejected: %s", result.reason.c_str());
        } else {
          RCLCPP_INFO(get_logger(), "max_speed updated to %ld", p.as_int());
        }
      }
    }
    return result;
  }

  OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParamsNode>());
  rclcpp::shutdown();
  return 0;
}
