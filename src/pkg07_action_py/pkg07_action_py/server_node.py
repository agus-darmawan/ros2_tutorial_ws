#!/usr/bin/env python3
"""Action server for /count_until in Python."""

import time
import rclpy
from rclpy.action import ActionServer, CancelResponse, GoalResponse
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.node import Node

from pkg03_tutorial_interfaces.action import CountUntil


class CountUntilServer(Node):
    def __init__(self):
        super().__init__('count_until_server_py')

        # ReentrantCallbackGroup lets the server handle goals concurrently
        # (needed because execute_callback is long-running).
        self._action_server = ActionServer(
            self,
            CountUntil,
            'count_until',
            execute_callback=self.execute_callback,
            goal_callback=self.goal_callback,
            cancel_callback=self.cancel_callback,
            callback_group=ReentrantCallbackGroup())

        self.get_logger().info('Action server /count_until ready (Python)')

    def goal_callback(self, goal_request):
        self.get_logger().info(
            f'Received goal: target={goal_request.target_number}, '
            f'period={goal_request.period}')
        if goal_request.target_number <= 0:
            self.get_logger().warn('Rejected: target must be > 0')
            return GoalResponse.REJECT
        return GoalResponse.ACCEPT

    def cancel_callback(self, goal_handle):
        self.get_logger().info('Cancel requested.')
        return CancelResponse.ACCEPT

    def execute_callback(self, goal_handle):
        goal = goal_handle.request
        feedback_msg = CountUntil.Feedback()
        counter = 0

        for i in range(1, goal.target_number + 1):
            if goal_handle.is_cancel_requested:
                goal_handle.canceled()
                result = CountUntil.Result()
                result.reached_number = counter
                self.get_logger().info(f'Canceled at {counter}')
                return result

            counter = i
            feedback_msg.current_number = counter
            goal_handle.publish_feedback(feedback_msg)
            self.get_logger().info(f'Feedback: {counter}')

            time.sleep(goal.period)

        goal_handle.succeed()
        result = CountUntil.Result()
        result.reached_number = counter
        self.get_logger().info(f'Succeeded: reached {counter}')
        return result


def main(args=None):
    rclpy.init(args=args)
    node = CountUntilServer()
    # MultiThreadedExecutor lets goal_callback and execute_callback run together
    executor = MultiThreadedExecutor()
    try:
        rclpy.spin(node, executor=executor)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
