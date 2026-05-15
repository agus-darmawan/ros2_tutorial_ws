// static_broadcaster.cpp
// Publishes a single, never-changing transform from `world` to `sensor_static`.
// Static transforms are sent once on a latched topic and remain available forever.

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

class StaticBroadcaster : public rclcpp::Node
{
public:
  StaticBroadcaster() : Node("static_broadcaster")
  {
    broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

    geometry_msgs::msg::TransformStamped t;
    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "world";         // parent
    t.child_frame_id  = "sensor_static"; // child

    // translation
    t.transform.translation.x = 1.0;
    t.transform.translation.y = 2.0;
    t.transform.translation.z = 0.5;

    // identity rotation (quaternion w=1, x=y=z=0)
    t.transform.rotation.x = 0.0;
    t.transform.rotation.y = 0.0;
    t.transform.rotation.z = 0.0;
    t.transform.rotation.w = 1.0;

    broadcaster_->sendTransform(t);
    RCLCPP_INFO(get_logger(),
      "Published static transform world -> sensor_static");
  }

private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StaticBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
