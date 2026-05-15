# 07 — Actions in Python

Python twin of `pkg06_action_cpp`. Same `/count_until` action, same interface.

## Key API (rclpy)

```python
from rclpy.action import ActionServer, ActionClient
from rclpy.action import CancelResponse, GoalResponse

# SERVER
self._action_server = ActionServer(
    self, ActionType, 'name',
    execute_callback=self.execute,     # required: do the work
    goal_callback=self.goal_cb,        # optional: accept/reject
    cancel_callback=self.cancel_cb)    # optional: handle cancel

# CLIENT
self._client = ActionClient(self, ActionType, 'name')
goal_future = self._client.send_goal_async(goal, feedback_callback=...)
result_future = goal_handle.get_result_async()
```

## Why `MultiThreadedExecutor`?

The server's `execute_callback` is long-running (it loops with `time.sleep`). If you use the default single-threaded executor, the node can't process new goals or cancels while executing. The fix:

```python
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup

# Pass callback_group=ReentrantCallbackGroup() to the ActionServer.
# Spin with a MultiThreadedExecutor.
```

This is **the** classic Python action pitfall. Single-threaded action servers in Python will appear to hang on cancel requests.

## Build & run

```bash
colcon build --packages-select pkg03_tutorial_interfaces pkg07_action_py
source install/setup.bash

ros2 run pkg07_action_py server_node   # terminal 1
ros2 run pkg07_action_py client_node 10 0.5   # terminal 2
```

## Cross-language

```bash
ros2 run pkg06_action_cpp server_node      # C++ server
ros2 run pkg07_action_py  client_node 5 1  # Python client
```

## Exercise

Add an `abort` condition: if `current_number` reaches 7, call `goal_handle.abort()` and return a result with `reached_number = -1`. Verify with the CLI.
