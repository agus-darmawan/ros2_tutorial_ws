// publisher_node.cpp
// A minimal ROS2 publisher in C++.
// Publishes a counter as std_msgs/String on the topic "/chatter" at 2 Hz.

#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class PublisherNode : public rclcpp::Node
{
public:
  PublisherNode()
  : Node("publisher_node"), count_(0)
  {
    // Create a publisher:
    //   topic name   : "chatter"
    //   message type : std_msgs/msg/String
    //   QoS depth    : 10 (history buffer size)
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // A wall timer fires every 500 ms and calls publish_message().
    timer_ = this->create_wall_timer(
      500ms, std::bind(&PublisherNode::publish_message, this));

    RCLCPP_INFO(this->get_logger(), "Publisher node started, publishing on /chatter");
  }

private:
  void publish_message()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello ROS2 from C++ #" + std::to_string(count_++);

    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  // Initialize the ROS2 client library
  rclcpp::init(argc, argv);

  // Spin keeps the node alive and processes callbacks
  rclcpp::spin(std::make_shared<PublisherNode>());

  // Cleanup
  rclcpp::shutdown();
  return 0;
}
