#!/usr/bin/env python3
"""Service server in Python. Adds two integers."""

import rclpy
from rclpy.node import Node
from pkg03_tutorial_interfaces.srv import AddTwoInts


class ServerNode(Node):
    def __init__(self):
        super().__init__('add_two_ints_server_py')

        # create_service(srv_type, srv_name, callback)
        self.srv = self.create_service(
            AddTwoInts, 'add_two_ints', self.handle_request)

        self.get_logger().info('Service /add_two_ints is ready (Python)')

    def handle_request(self, request, response):
        # The framework gives us a pre-allocated response object.
        # Fill its fields and return it.
        response.sum = request.a + request.b
        self.get_logger().info(
            f'Request: a={request.a}, b={request.b} -> sum={response.sum}')
        return response


def main(args=None):
    rclpy.init(args=args)
    node = ServerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
