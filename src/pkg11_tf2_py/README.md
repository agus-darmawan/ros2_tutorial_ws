# 11 — TF2 in Python

Python twin of `pkg10_tf2_cpp`. Read that README first for the conceptual background — this one only highlights the Python API.

## Key API (rclpy)

```python
from tf2_ros import TransformBroadcaster, TransformException
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
from geometry_msgs.msg import TransformStamped

# BROADCAST
self.br = TransformBroadcaster(self)
t = TransformStamped()
t.header.stamp = self.get_clock().now().to_msg()
t.header.frame_id = 'parent'
t.child_frame_id  = 'child'
# fill translation + rotation ...
self.br.sendTransform(t)

# LISTEN
self.buffer   = Buffer()
self.listener = TransformListener(self.buffer, self)
t = self.buffer.lookup_transform('world', 'robot', rclpy.time.Time())
```

## Build & run

```bash
colcon build --packages-select pkg11_tf2_py
source install/setup.bash

# In 3 terminals
ros2 run pkg11_tf2_py static_broadcaster
ros2 run pkg11_tf2_py dynamic_broadcaster
ros2 run pkg11_tf2_py listener
```

## Notes on quaternions

The Python `tf_transformations` package is the easiest helper, but it isn't always installed. Our listener implements `yaw_from_quaternion` by hand to stay dependency-free. For production code, prefer:

```bash
sudo apt install ros-humble-tf-transformations
```
```python
from tf_transformations import euler_from_quaternion, quaternion_from_euler
```

## Cross-language

The C++ broadcaster and Python listener (or vice versa) interoperate freely. Try:

```bash
ros2 run pkg10_tf2_cpp dynamic_broadcaster   # terminal 1
ros2 run pkg11_tf2_py  listener              # terminal 2
```
