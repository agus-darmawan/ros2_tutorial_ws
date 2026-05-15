// listener.cpp
// Queries the TF tree: every 0.5 s, look up the transform from "world" to "robot"
// and print the (x, y, yaw) of the robot.

#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/exceptions.h"
#include "tf2/utils.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class TfListener : public rclcpp::Node
{
public:
  TfListener() : Node("tf_listener")
  {
    buffer_   = std::make_unique<tf2_ros::Buffer>(get_clock());
    listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);
    timer_    = create_wall_timer(500ms, std::bind(&TfListener::lookup, this));
  }

private:
  void lookup()
  {
    geometry_msgs::msg::TransformStamped t;
    try {
      // lookupTransform(target_frame, source_frame, time)
      // tf2::TimePointZero = "latest available"
      t = buffer_->lookupTransform("world", "robot", tf2::TimePointZero);
    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN(get_logger(), "Could not transform: %s", ex.what());
      return;
    }

    double yaw = tf2::getYaw(t.transform.rotation);
    RCLCPP_INFO(get_logger(),
      "robot pose in world: x=%.2f y=%.2f yaw=%.2f rad",
      t.transform.translation.x,
      t.transform.translation.y,
      yaw);
  }

  std::unique_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TfListener>());
  rclcpp::shutdown();
  return 0;
}
