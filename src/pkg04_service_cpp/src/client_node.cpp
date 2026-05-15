// client_node.cpp
// Calls /add_two_ints once with values taken from command line args
// (or 3 and 5 by default), prints the response, and exits.

#include <chrono>
#include <cstdlib>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "pkg03_tutorial_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = pkg03_tutorial_interfaces::srv::AddTwoInts;
using namespace std::chrono_literals;

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("add_two_ints_client");
  auto client = node->create_client<AddTwoInts>("add_two_ints");

  // Wait until the server is up (or we get tired)
  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for service.");
      return 1;
    }
    RCLCPP_INFO(node->get_logger(), "Service not available yet, waiting...");
  }

  // Build the request
  auto request = std::make_shared<AddTwoInts::Request>();
  request->a = (argc > 1) ? std::atoll(argv[1]) : 3;
  request->b = (argc > 2) ? std::atoll(argv[2]) : 5;

  RCLCPP_INFO(node->get_logger(), "Sending: a=%ld, b=%ld", request->a, request->b);

  // async_send_request returns a future. We spin until it completes.
  auto future = client->async_send_request(request);

  if (rclcpp::spin_until_future_complete(node, future) ==
      rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(node->get_logger(), "Result: %ld", future.get()->sum);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to call /add_two_ints.");
  }

  rclcpp::shutdown();
  return 0;
}
