// client_node.cpp
// Sends a goal to /count_until and prints feedback + result.

#include <functional>
#include <memory>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "pkg03_tutorial_interfaces/action/count_until.hpp"

using CountUntil = pkg03_tutorial_interfaces::action::CountUntil;
using GoalHandle = rclcpp_action::ClientGoalHandle<CountUntil>;
using namespace std::chrono_literals;

class CountUntilClient : public rclcpp::Node
{
public:
  CountUntilClient() : Node("count_until_client")
  {
    client_ = rclcpp_action::create_client<CountUntil>(this, "count_until");
  }

  void send_goal(int target, float period)
  {
    if (!client_->wait_for_action_server(5s)) {
      RCLCPP_ERROR(get_logger(), "Action server not available.");
      rclcpp::shutdown();
      return;
    }

    auto goal = CountUntil::Goal();
    goal.target_number = target;
    goal.period = period;

    using namespace std::placeholders;
    auto options = rclcpp_action::Client<CountUntil>::SendGoalOptions();
    options.goal_response_callback =
      std::bind(&CountUntilClient::goal_response_cb, this, _1);
    options.feedback_callback =
      std::bind(&CountUntilClient::feedback_cb, this, _1, _2);
    options.result_callback =
      std::bind(&CountUntilClient::result_cb, this, _1);

    RCLCPP_INFO(get_logger(), "Sending goal: count to %d every %.2f s", target, period);
    client_->async_send_goal(goal, options);
  }

private:
  void goal_response_cb(const GoalHandle::SharedPtr & goal_handle)
  {
    if (!goal_handle) {
      RCLCPP_ERROR(get_logger(), "Goal rejected by server.");
    } else {
      RCLCPP_INFO(get_logger(), "Goal accepted.");
    }
  }

  void feedback_cb(
    GoalHandle::SharedPtr,
    const std::shared_ptr<const CountUntil::Feedback> feedback)
  {
    RCLCPP_INFO(get_logger(), "Feedback: current_number = %d",
                feedback->current_number);
  }

  void result_cb(const GoalHandle::WrappedResult & result)
  {
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        RCLCPP_INFO(get_logger(), "Result: reached_number = %d",
                    result.result->reached_number); break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(get_logger(), "Goal was aborted."); break;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_WARN(get_logger(), "Goal was canceled."); break;
      default:
        RCLCPP_ERROR(get_logger(), "Unknown result code.");
    }
    rclcpp::shutdown();
  }

  rclcpp_action::Client<CountUntil>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CountUntilClient>();

  int target = (argc > 1) ? std::atoi(argv[1]) : 5;
  float period = (argc > 2) ? std::atof(argv[2]) : 1.0f;

  node->send_goal(target, period);
  rclcpp::spin(node);
  return 0;
}
