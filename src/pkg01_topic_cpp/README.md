# 01 — Topics in C++

## What is a Topic?

A **topic** is a named, asynchronous, many-to-many data stream.

- A node that sends data on a topic is a **publisher**.
- A node that receives data is a **subscriber**.
- Publishers and subscribers don't know about each other — they only agree on a **topic name** and a **message type**.

```
 [publisher_node]  --(std_msgs/String)-->  /chatter  --->  [subscriber_node]
                                            topic
```

**Use topics for:** sensor readings, robot velocity commands, images, anything that is a *stream of data over time*.

## What this package contains

| File | Role |
|------|------|
| `src/publisher_node.cpp` | Publishes `std_msgs/String` on `/chatter` at 2 Hz |
| `src/subscriber_node.cpp` | Subscribes to `/chatter` and logs every message |

## Key API in C++

```cpp
// Inside your Node:
publisher_ = this->create_publisher<MsgType>("topic_name", qos_depth);
publisher_->publish(message);

subscription_ = this->create_subscription<MsgType>(
    "topic_name", qos_depth, callback);

// Timer that fires periodically:
timer_ = this->create_wall_timer(500ms, callback);
```

Every ROS2 C++ program follows the same skeleton:

```cpp
int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);                          // 1. Init
  rclcpp::spin(std::make_shared<MyNode>());          // 2. Spin
  rclcpp::shutdown();                                // 3. Shutdown
}
```

`spin()` blocks the program and processes incoming messages, timers, and services. Without it, callbacks never run.

## How to build & run

From the workspace root:

```bash
colcon build --packages-select pkg01_topic_cpp
source install/setup.bash
```

In **terminal 1**:
```bash
ros2 run pkg01_topic_cpp publisher_node
```

In **terminal 2**:
```bash
ros2 run pkg01_topic_cpp subscriber_node
```

## Inspect with the CLI

```bash
ros2 topic list                  # see all active topics
ros2 topic info /chatter         # type and connection count
ros2 topic echo /chatter         # print live messages
ros2 topic hz /chatter           # measure publishing rate
ros2 node list                   # see all running nodes
rqt_graph                        # GUI: see the topic graph
```

## Exercise

1. Change the publish rate from 500ms to 100ms — what does `ros2 topic hz` report?
2. Run **two** subscribers at once. Both should receive the same messages. Why? (Hint: many-to-many.)
3. Kill the publisher while a subscriber is running. The subscriber stays alive — topics are decoupled.
