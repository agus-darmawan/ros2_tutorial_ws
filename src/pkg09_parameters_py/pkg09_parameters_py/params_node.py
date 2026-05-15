#!/usr/bin/env python3
"""Demonstrates declaring, reading, and reacting to parameters in Python."""

import rclpy
from rclpy.node import Node
from rcl_interfaces.msg import SetParametersResult


class ParamsNode(Node):
    def __init__(self):
        super().__init__('params_node_py')

        # 1. Declare with default values
        self.declare_parameter('robot_name', 'turtle')
        self.declare_parameter('max_speed', 10)
        self.declare_parameter('loop_rate_hz', 1.0)
        self.declare_parameter('waypoints', ['A', 'B', 'C'])

        # 2. Read at startup
        robot_name = self.get_parameter('robot_name').value
        max_speed = self.get_parameter('max_speed').value
        rate = self.get_parameter('loop_rate_hz').value
        waypoints = self.get_parameter('waypoints').value

        self.get_logger().info(
            f'robot_name={robot_name}, max_speed={max_speed}, '
            f'loop_rate_hz={rate}')
        for wp in waypoints:
            self.get_logger().info(f'  waypoint: {wp}')

        # 3. React to runtime changes
        self.add_on_set_parameters_callback(self.on_param_change)

        # Show current value every 2s
        self.timer = self.create_timer(2.0, self.print_current)

    def on_param_change(self, params):
        result = SetParametersResult(successful=True)
        for p in params:
            if p.name == 'max_speed':
                if p.value < 0 or p.value > 100:
                    result.successful = False
                    result.reason = 'max_speed must be in [0, 100]'
                    self.get_logger().warn(f'Rejected: {result.reason}')
                else:
                    self.get_logger().info(f'max_speed updated to {p.value}')
        return result

    def print_current(self):
        self.get_logger().info(
            f'Current max_speed = {self.get_parameter("max_speed").value}')


def main(args=None):
    rclpy.init(args=args)
    node = ParamsNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
