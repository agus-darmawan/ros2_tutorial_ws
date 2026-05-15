#!/usr/bin/env python3
"""
A minimal ROS2 subscriber in Python.
Subscribes to /chatter and logs every received message.
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class SubscriberNode(Node):
    def __init__(self):
        super().__init__('subscriber_node_py')

        # Create subscriber. Same topic name & type as the publisher.
        self.subscription = self.create_subscription(
            String,
            'chatter',
            self.listener_callback,
            10)

        self.get_logger().info('Subscriber node (Python) listening on /chatter')

    def listener_callback(self, msg: String):
        self.get_logger().info(f'Received: "{msg.data}"')


def main(args=None):
    rclpy.init(args=args)
    node = SubscriberNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
