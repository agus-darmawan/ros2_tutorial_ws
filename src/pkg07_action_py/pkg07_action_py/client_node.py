#!/usr/bin/env python3
"""Action client for /count_until in Python."""

import sys
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node

from pkg03_tutorial_interfaces.action import CountUntil


class CountUntilClient(Node):
    def __init__(self):
        super().__init__('count_until_client_py')
        self._client = ActionClient(self, CountUntil, 'count_until')

    def send_goal(self, target, period):
        if not self._client.wait_for_server(timeout_sec=5.0):
            self.get_logger().error('Action server not available.')
            return

        goal_msg = CountUntil.Goal()
        goal_msg.target_number = target
        goal_msg.period = period

        self.get_logger().info(f'Sending goal: count to {target} every {period}s')

        # Async: returns a future for the goal handle
        send_goal_future = self._client.send_goal_async(
            goal_msg, feedback_callback=self.feedback_callback)
        send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error('Goal rejected.')
            rclpy.shutdown()
            return
        self.get_logger().info('Goal accepted.')
        result_future = goal_handle.get_result_async()
        result_future.add_done_callback(self.result_callback)

    def feedback_callback(self, feedback_msg):
        self.get_logger().info(
            f'Feedback: current_number = {feedback_msg.feedback.current_number}')

    def result_callback(self, future):
        result = future.result().result
        self.get_logger().info(f'Result: reached_number = {result.reached_number}')
        rclpy.shutdown()


def main(args=None):
    rclpy.init(args=args)
    node = CountUntilClient()

    target = int(sys.argv[1]) if len(sys.argv) > 1 else 5
    period = float(sys.argv[2]) if len(sys.argv) > 2 else 1.0

    node.send_goal(target, period)

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
