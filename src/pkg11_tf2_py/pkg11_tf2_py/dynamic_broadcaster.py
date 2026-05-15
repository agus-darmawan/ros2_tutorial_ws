#!/usr/bin/env python3
"""Dynamic TF broadcaster in Python: world -> robot circling at 20 Hz."""

import math
import rclpy
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
from geometry_msgs.msg import TransformStamped


def quaternion_from_yaw(yaw: float):
    """Return (x, y, z, w) for a rotation about the z-axis."""
    return (0.0, 0.0, math.sin(yaw / 2.0), math.cos(yaw / 2.0))


class DynamicBroadcaster(Node):
    def __init__(self):
        super().__init__('dynamic_broadcaster_py')
        self.br = TransformBroadcaster(self)
        self.timer = self.create_timer(0.05, self.tick)
        self.t = 0.0
        self.get_logger().info('Publishing dynamic TF world -> robot')

    def tick(self):
        tf = TransformStamped()
        tf.header.stamp = self.get_clock().now().to_msg()
        tf.header.frame_id = 'world'
        tf.child_frame_id = 'robot'

        tf.transform.translation.x = math.cos(self.t)
        tf.transform.translation.y = math.sin(self.t)
        tf.transform.translation.z = 0.0

        qx, qy, qz, qw = quaternion_from_yaw(self.t + math.pi / 2)
        tf.transform.rotation.x = qx
        tf.transform.rotation.y = qy
        tf.transform.rotation.z = qz
        tf.transform.rotation.w = qw

        self.br.sendTransform(tf)
        self.t += 0.05


def main(args=None):
    rclpy.init(args=args)
    node = DynamicBroadcaster()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
