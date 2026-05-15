# 01 — Topics in C++ (with QoS)

## What is a Topic?

A **topic** is a named, asynchronous, many-to-many data stream.

* A node that sends data is a **publisher**
* A node that receives data is a **subscriber**
* They do not know each other directly — they only agree on:

  * topic name
  * message type
  * QoS (Quality of Service)

```
 [publisher_node]  --(std_msgs/String)-->  /chatter  --->  [subscriber_node]
```

**Use topics for:** sensor data, robot velocity commands, images, telemetry, and streaming data.

---

## 🧠 What is QoS in ROS 2 Topics?

**QoS (Quality of Service)** defines the communication behavior between publisher and subscriber in ROS 2.

It controls:

* whether messages must arrive or can be dropped
* how many messages are stored in a buffer
* whether late subscribers can receive past data
* reliability vs latency trade-offs

👉 In ROS 2, **a topic connection is only established if QoS settings are compatible**

---

## 🔧 Key QoS Policies

### 1. Reliability

```
RELIABLE     → guarantees delivery (retries enabled)
BEST_EFFORT  → messages may be dropped (lower latency)
```

* Use RELIABLE for critical data (e.g. `/cmd_vel`)
* Use BEST_EFFORT for real-time sensors (e.g. camera, LiDAR)

---

### 2. Durability

```
VOLATILE         → no history for late subscribers
TRANSIENT_LOCAL  → last message is saved and sent to new subscribers
```

* Use TRANSIENT_LOCAL for state information (e.g. robot status)

---

### 3. History + Depth

```
KEEP_LAST → stores only last N messages
depth = N
```

Example: depth = 10 → keep last 10 messages

---

## ⚙️ QoS in C++ (ROS 2)

```cpp
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

rclcpp::QoS qos_profile(10);
qos_profile.reliable();
qos_profile.durability_volatile();
```

---

## 🔌 Publisher and Subscriber with QoS

### Publisher

```cpp
publisher_ = this->create_publisher<std_msgs::msg::String>(
    "chatter",
    rclcpp::QoS(10).reliable()
);
```

### Subscriber

```cpp
subscription_ = this->create_subscription<std_msgs::msg::String>(
    "chatter",
    rclcpp::QoS(10).reliable(),
    std::bind(&MyNode::callback, this, std::placeholders::_1)
);
```

---

## ⚠️ Important: QoS Must Match

If QoS is incompatible:

```
Publisher: BEST_EFFORT
Subscriber: RELIABLE
❌ No connection
```

or

```
Publisher: RELIABLE
Subscriber: BEST_EFFORT
❌ No connection
```

👉 This is one of the most common ROS 2 issues

---

## 📌 Common QoS Profiles by Use Case

| Use Case         | QoS Configuration          |
| ---------------- | -------------------------- |
| Camera / LiDAR   | BEST_EFFORT + KEEP_LAST    |
| Robot commands   | RELIABLE                   |
| Status / battery | RELIABLE + TRANSIENT_LOCAL |
| Map data         | TRANSIENT_LOCAL            |

---

## 🧩 ROS 2 Topic API (C++)

```cpp
publisher_ = this->create_publisher<MsgType>("topic_name", qos);
publisher_->publish(message);

subscription_ = this->create_subscription<MsgType>(
    "topic_name",
    qos,
    callback);

timer_ = this->create_wall_timer(500ms, callback);
```

---

## 🧠 ROS 2 Node Skeleton

```cpp
int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MyNode>());
  rclcpp::shutdown();
}
```

* `spin()` is required to process callbacks

---

## 🚀 Build & Run

```bash
colcon build --packages-select pkg01_topic_cpp
source install/setup.bash
```

Terminal 1:

```bash
ros2 run pkg01_topic_cpp publisher_node
```

Terminal 2:

```bash
ros2 run pkg01_topic_cpp subscriber_node
```

---

## 🔍 Debugging Tools

```bash
ros2 topic list
ros2 topic info /chatter
ros2 topic echo /chatter
ros2 topic hz /chatter
rqt_graph
```

---

## 🧪 Exercises

1. Change publish rate from 500ms → 100ms

   * Observe `ros2 topic hz`

2. Run two subscribers

   * Both receive the same messages (why?)

3. Stop publisher

   * Subscriber still runs (decoupled system)

4. QoS experiment:

   * Publisher: BEST_EFFORT
   * Subscriber: RELIABLE
     → Observe no connection

---

## 🔥 Key Takeaway

ROS 2 topics are **decoupled, asynchronous streams**, and QoS is what defines *how reliable and how real-time the communication behaves*.
