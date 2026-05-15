# 02 — Topics in Python

This is the **Python twin** of `pkg01_topic_cpp`. Same concept, different language.

## C++ vs Python — the differences you actually care about

| | C++ (rclcpp) | Python (rclpy) |
|--|------------|---------------|
| Build system | `ament_cmake` + `CMakeLists.txt` | `ament_python` + `setup.py` |
| Create node | `class : public rclcpp::Node` | `class(Node):` |
| Init / shutdown | `rclcpp::init / shutdown` | `rclpy.init / shutdown` |
| Spin | `rclcpp::spin(node)` | `rclpy.spin(node)` |
| Logger | `RCLCPP_INFO(get_logger(), ...)` | `self.get_logger().info(...)` |
| Timer | `create_wall_timer(500ms, cb)` | `create_timer(0.5, cb)` |

> Both can talk to each other — a C++ publisher and a Python subscriber on the same topic is completely normal. ROS2 is language-agnostic at the wire level (DDS).

## Build & run

```bash
colcon build --packages-select pkg02_topic_py
source install/setup.bash

# Terminal 1
ros2 run pkg02_topic_py publisher_node

# Terminal 2
ros2 run pkg02_topic_py subscriber_node
```

## Cross-language demo

Try this to *prove* that the language doesn't matter:

```bash
# Terminal 1: C++ publisher
ros2 run pkg01_topic_cpp publisher_node

# Terminal 2: Python subscriber
ros2 run pkg02_topic_py subscriber_node
```

The Python subscriber will happily receive messages from the C++ publisher because they share the topic name `/chatter` and the message type `std_msgs/String`.

## Exercise

1. Make the Python publisher send a `geometry_msgs/Twist` instead of a `String`. Run `ros2 topic echo /cmd_vel` to verify.
2. Run **two publishers** with different node names but the same topic name. The subscriber will receive both — order is not guaranteed.
