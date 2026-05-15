#!/usr/bin/env python3
"""Service client in Python. Calls /add_two_ints once and exits."""

import sys
import rclpy
from rclpy.node import Node
from pkg03_tutorial_interfaces.srv import AddTwoInts


class ClientNode(Node):
    def __init__(self):
        super().__init__('add_two_ints_client_py')
        self.client = self.create_client(AddTwoInts, 'add_two_ints')

        # Block until server is available
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Service not available yet, waiting...')

    def send_request(self, a, b):
        request = AddTwoInts.Request()
        request.a = a
        request.b = b
        # Returns a Future. spin_until_future_complete blocks until done.
        future = self.client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        return future.result()


def main(args=None):
    rclpy.init(args=args)
    node = ClientNode()

    a = int(sys.argv[1]) if len(sys.argv) > 1 else 3
    b = int(sys.argv[2]) if len(sys.argv) > 2 else 5

    node.get_logger().info(f'Sending: a={a}, b={b}')
    response = node.send_request(a, b)
    node.get_logger().info(f'Result: {response.sum}')

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
