// server_node.cpp
// Exposes the service /add_two_ints. When a client sends two integers,
// the server replies with their sum.

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "pkg03_tutorial_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = pkg03_tutorial_interfaces::srv::AddTwoInts;

class ServerNode : public rclcpp::Node
{
public:
  ServerNode() : Node("add_two_ints_server")
  {
    service_ = this->create_service<AddTwoInts>(
      "add_two_ints",
      std::bind(&ServerNode::handle_request, this,
                std::placeholders::_1, std::placeholders::_2));

    RCLCPP_INFO(this->get_logger(), "Service /add_two_ints is ready.");
  }

private:
  void handle_request(
    const std::shared_ptr<AddTwoInts::Request> request,
    std::shared_ptr<AddTwoInts::Response> response)
  {
    response->sum = request->a + request->b;
    RCLCPP_INFO(this->get_logger(),
                "Request: a=%ld, b=%ld -> sum=%ld",
                request->a, request->b, response->sum);
  }

  rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ServerNode>());
  rclcpp::shutdown();
  return 0;
}
