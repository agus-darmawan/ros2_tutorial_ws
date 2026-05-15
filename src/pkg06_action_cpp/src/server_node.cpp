// server_node.cpp
// Action server for /count_until.
// Goal:     target_number, period
// Feedback: current_number (sent every `period` seconds)
// Result:   reached_number (sent once at the end)

#include <functional>
#include <memory>
#include <thread>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "pkg03_tutorial_interfaces/action/count_until.hpp"

using CountUntil = pkg03_tutorial_interfaces::action::CountUntil;
using GoalHandle = rclcpp_action::ServerGoalHandle<CountUntil>;

class CountUntilServer : public rclcpp::Node
{
public:
  CountUntilServer() : Node("count_until_server")
  {
    using namespace std::placeholders;

    action_server_ = rclcpp_action::create_server<CountUntil>(
      this,
      "count_until",
      std::bind(&CountUntilServer::handle_goal, this, _1, _2),
      std::bind(&CountUntilServer::handle_cancel, this, _1),
      std::bind(&CountUntilServer::handle_accepted, this, _1));

    RCLCPP_INFO(this->get_logger(), "Action server /count_until ready.");
  }

private:
  // 1. Decide whether to accept the goal
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID & /*uuid*/,
    std::shared_ptr<const CountUntil::Goal> goal)
  {
    RCLCPP_INFO(this->get_logger(),
      "Received goal: target=%d, period=%.2f",
      goal->target_number, goal->period);

    if (goal->target_number <= 0) {
      RCLCPP_WARN(this->get_logger(), "Rejected: target must be > 0.");
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  // 2. Handle a cancel request from the client
  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandle> /*goal_handle*/)
  {
    RCLCPP_INFO(this->get_logger(), "Cancel requested by client.");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  // 3. Once accepted, do the actual work in a new thread so we don't block.
  void handle_accepted(const std::shared_ptr<GoalHandle> goal_handle)
  {
    std::thread{std::bind(&CountUntilServer::execute, this, goal_handle)}.detach();
  }

  void execute(const std::shared_ptr<GoalHandle> goal_handle)
  {
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<CountUntil::Feedback>();
    auto result = std::make_shared<CountUntil::Result>();

    int counter = 0;
    rclcpp::Rate loop_rate(1.0 / goal->period);

    for (int i = 1; i <= goal->target_number && rclcpp::ok(); ++i) {
      // Check if the client canceled
      if (goal_handle->is_canceling()) {
        result->reached_number = counter;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "Goal canceled at %d", counter);
        return;
      }

      counter = i;
      feedback->current_number = counter;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "Feedback: %d", counter);

      loop_rate.sleep();
    }

    result->reached_number = counter;
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "Goal succeeded: reached %d", counter);
  }

  rclcpp_action::Server<CountUntil>::SharedPtr action_server_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CountUntilServer>());
  rclcpp::shutdown();
  return 0;
}
