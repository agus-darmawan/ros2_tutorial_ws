// dynamic_broadcaster.cpp
// Publishes a transform that changes over time: the child frame "robot"
// rotates around the origin of "world" on a 1m radius circle.

#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class DynamicBroadcaster : public rclcpp::Node
{
public:
  DynamicBroadcaster() : Node("dynamic_broadcaster"), t_(0.0)
  {
    broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    timer_ = create_wall_timer(50ms,
      std::bind(&DynamicBroadcaster::tick, this));
    RCLCPP_INFO(get_logger(), "Publishing dynamic TF world -> robot");
  }

private:
  void tick()
  {
    geometry_msgs::msg::TransformStamped tf;
    tf.header.stamp = now();
    tf.header.frame_id = "world";
    tf.child_frame_id  = "robot";

    tf.transform.translation.x = std::cos(t_);
    tf.transform.translation.y = std::sin(t_);
    tf.transform.translation.z = 0.0;

    // Yaw rotation = t_ radians, so the robot always "faces forward" along its circle
    tf2::Quaternion q;
    q.setRPY(0, 0, t_ + M_PI_2);

    tf.transform.rotation.x = q.x();
    tf.transform.rotation.y = q.y();
    tf.transform.rotation.z = q.z();
    tf.transform.rotation.w = q.w();

    broadcaster_->sendTransform(tf);
    t_ += 0.05;
  }

  std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
  double t_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DynamicBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
