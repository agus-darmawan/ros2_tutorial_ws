#!/usr/bin/env python3
"""
A minimal ROS2 publisher in Python.
Publishes a counter as std_msgs/String on the topic /chatter at 2 Hz.
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class PublisherNode(Node):
    def __init__(self):
        super().__init__('publisher_node_py')

        # Create publisher:
        #   message type : String
        #   topic name   : 'chatter'
        #   QoS depth    : 10
        self.publisher_ = self.create_publisher(String, 'chatter', 10)

        # Timer fires every 0.5 s and calls timer_callback
        self.timer = self.create_timer(0.5, self.timer_callback)
        self.count = 0

        self.get_logger().info('Publisher node (Python) started on /chatter')

    def timer_callback(self):
        msg = String()
        msg.data = f'Hello ROS2 from Python #{self.count}'
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: "{msg.data}"')
        self.count += 1


def main(args=None):
    rclpy.init(args=args)            # 1. Init
    node = PublisherNode()
    try:
        rclpy.spin(node)             # 2. Spin (blocks)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()             # 3. Shutdown


if __name__ == '__main__':
    main()
