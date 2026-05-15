# 06 — Actions in C++

## What is an Action?

An **action** is for **long-running, cancellable** tasks where the client wants:
1. To send a **goal** (what to do).
2. To receive **feedback** while it runs.
3. To get a **final result** when it's done.
4. The ability to **cancel** mid-execution.

Think: navigate to a point, scan an area, calibrate a sensor, follow a trajectory.

```
                       send_goal
   [client]  ----------------------------->  [server]
            <-- "accepted / rejected" ------
            <-- feedback (many times)  -----
            <-- result (once at end)   -----
            -- cancel request --(opt)-->
```

Under the hood, actions are built on **topics + services**. You don't need to know this, but it explains why they support multiple goals and cancellation.

## When to use which

| Need | Use |
|------|-----|
| Stream of sensor data | Topic |
| Quick yes/no answer | Service |
| Slow task with progress / cancel | **Action** |

## What's in this package

| File | Role |
|------|------|
| `src/server_node.cpp` | Server: counts to `target_number`, one count per `period` seconds |
| `src/client_node.cpp` | Client: sends goal, prints feedback, prints result |

Interface: `pkg03_tutorial_interfaces/action/CountUntil`:
```
int32 target_number      # goal
float32 period
---
int32 reached_number     # result
---
int32 current_number     # feedback (every period seconds)
```

## Server lifecycle (3 callbacks)

```cpp
// 1. Should we accept this goal at all?
GoalResponse handle_goal(uuid, goal);          // ACCEPT_AND_EXECUTE / REJECT

// 2. Client wants to cancel — should we let them?
CancelResponse handle_cancel(goal_handle);     // ACCEPT / REJECT

// 3. Goal accepted — do the work (in a separate thread!)
void handle_accepted(goal_handle);             // dispatch to worker thread
```

In the worker, you must:
- Periodically `goal_handle->publish_feedback(feedback)`.
- Check `goal_handle->is_canceling()` and call `goal_handle->canceled(result)` if true.
- Call `goal_handle->succeed(result)` or `goal_handle->abort(result)` at the end.

## Build & run

```bash
colcon build --packages-select pkg03_tutorial_interfaces pkg06_action_cpp
source install/setup.bash
```

**Terminal 1:**
```bash
ros2 run pkg06_action_cpp server_node
```

**Terminal 2:**
```bash
ros2 run pkg06_action_cpp client_node 10 0.5
# counts 1..10, one number every 0.5 s
```

## Inspect with CLI

```bash
ros2 action list
ros2 action info /count_until
ros2 action send_goal /count_until pkg03_tutorial_interfaces/action/CountUntil \
  "{target_number: 5, period: 1.0}" --feedback
```

The `--feedback` flag streams feedback to the terminal.

## Exercise

1. Reject goals with `target_number > 1000`. Test it.
2. Use the CLI to send a goal, then `Ctrl+C` the CLI before it finishes — observe the server's cancel handler firing.
