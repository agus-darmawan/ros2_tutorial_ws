# ROS2 Humble Tutorial Workspace

A complete teaching workspace covering the fundamentals of **ROS2 Humble** with both **C++ (rclcpp)** and **Python (rclpy)** examples.

Each concept lives in its own package. Build them all at once with `colcon build`, or build a single one with `colcon build --packages-select <name>`.

**Author:** agus-darmawan &lt;darmawandeveloper@gmail.com&gt;
**License:** BSD-3-Clause (see `LICENSE`)

## Curriculum overview

Follow the suggested order. Each package is prefixed `pkgNN_` so the lessons appear in order in the filesystem. The prefix is part of the actual ROS2 package name — it's how you import, build, and run them.

| # | Package | Concept | Language |
|---|---------|---------|----------|
| 1 | `pkg01_topic_cpp` | Publisher / Subscriber | C++ |
| 2 | `pkg02_topic_py` | Publisher / Subscriber | Python |
| 3 | `pkg03_tutorial_interfaces` | Custom msg / srv / action definitions | IDL |
| 4 | `pkg04_service_cpp` | Services (client / server) | C++ |
| 5 | `pkg05_service_py` | Services (client / server) | Python |
| 6 | `pkg06_action_cpp` | Actions (long-running tasks) | C++ |
| 7 | `pkg07_action_py` | Actions (long-running tasks) | Python |
| 8 | `pkg08_parameters_cpp` | Node parameters | C++ |
| 9 | `pkg09_parameters_py` | Node parameters | Python |
| 10 | `pkg10_tf2_cpp` | TF2 — coordinate frames | C++ |
| 11 | `pkg11_tf2_py` | TF2 — coordinate frames | Python |
| 12 | `pkg12_launch_examples` | Launch files (minimal, args, namespaces) | Python launch |

> Build `pkg03_tutorial_interfaces` **before** the service/action packages — they depend on it. `colcon build` figures out the order automatically.

## Prerequisites

```bash
# Ubuntu 22.04 + ROS2 Humble
source /opt/ros/humble/setup.bash

sudo apt install \
  ros-humble-rclcpp \
  ros-humble-rclpy \
  ros-humble-std-msgs \
  ros-humble-geometry-msgs \
  ros-humble-sensor-msgs \
  ros-humble-tf2 \
  ros-humble-tf2-ros \
  ros-humble-tf2-geometry-msgs \
  ros-humble-turtlesim \
  python3-colcon-common-extensions
```

## Build the whole workspace

```bash
cd ~/ros2_tutorial_ws
colcon build --symlink-install
source install/setup.bash
```

`--symlink-install` lets you edit Python files and re-run without rebuilding.

## How to use this material

1. Start with `pkg01_topic_cpp`, then `pkg02_topic_py`. Read the README **first**.
2. Build `pkg03_tutorial_interfaces` before moving to services/actions.
3. Run each example in 2–3 terminals — ROS2 nodes are independent processes.
4. Use `ros2 topic / service / action / node / param` CLI tools constantly. They are your debugger.

## ROS2 big picture (cheat sheet)

```
┌─────────────────────────────────────────────────────────────┐
│                       ROS2 Concepts                          │
├─────────────────────────────────────────────────────────────┤
│ Node       → a process that does one job                     │
│ Topic      → many-to-many async stream (sensor data)         │
│ Service    → one-to-one sync request/response (queries)      │
│ Action     → long-running task with feedback (navigation)    │
│ Parameter  → tunable values on a node                        │
│ TF2        → tree of coordinate frames over time             │
│ Launch     → start many nodes with config in one shot        │
└─────────────────────────────────────────────────────────────┘
```

**Rule of thumb for picking a primitive:**
- Continuous data flow → **Topic**
- Quick "give me an answer now" → **Service**
- "Do this for a while and tell me how it's going" → **Action**
- Robot poses / sensor mounting → **TF2**
- Numbers you want to tune without recompiling → **Parameter**

## CLI quick reference

```bash
ros2 node list / info <node>
ros2 topic list / info / echo / pub / hz
ros2 service list / type / call
ros2 action list / info / send_goal
ros2 param list / get / set
ros2 interface show <type>
ros2 launch <pkg> <file>
rqt_graph     # visualize the node/topic graph
rqt           # general GUI dashboard
```

## Suggested next topics (not in this workspace)

When the students are comfortable with the above, the natural next steps are:

- **Lifecycle (managed) nodes** — nodes with a state machine for clean startup/shutdown.
- **Executors & callback groups** — concurrency, callback isolation, real-time priorities.
- **QoS (Quality of Service)** profiles — reliability, durability, history depth for topics.
- **Composition** — running multiple nodes in one process for zero-copy speedup.
- **rosbag2** — recording and replaying topic data.
- **URDF + robot_state_publisher** — robot description and full TF tree publishing.
