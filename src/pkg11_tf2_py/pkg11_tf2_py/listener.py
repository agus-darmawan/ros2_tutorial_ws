#!/usr/bin/env python3
"""TF listener in Python: query world -> robot every 0.5 s."""

import math
import rclpy
from rclpy.node import Node
from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener


def yaw_from_quaternion(q):
    """Extract yaw (rotation about z) from a geometry_msgs Quaternion."""
    siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
    cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
    return math.atan2(siny_cosp, cosy_cosp)


class TfListener(Node):
    def __init__(self):
        super().__init__('tf_listener_py')
        self.buffer = Buffer()
        self.listener = TransformListener(self.buffer, self)
        self.timer = self.create_timer(0.5, self.lookup)

    def lookup(self):
        try:
            t = self.buffer.lookup_transform(
                'world', 'robot', rclpy.time.Time())
        except TransformException as ex:
            self.get_logger().warn(f'Could not transform: {ex}')
            return

        yaw = yaw_from_quaternion(t.transform.rotation)
        self.get_logger().info(
            f'robot pose in world: '
            f'x={t.transform.translation.x:.2f} '
            f'y={t.transform.translation.y:.2f} '
            f'yaw={yaw:.2f} rad')


def main(args=None):
    rclpy.init(args=args)
    node = TfListener()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
