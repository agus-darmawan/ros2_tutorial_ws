# 10 — TF2 in C++

## What is TF2?

**TF2** is the ROS2 system that tracks **coordinate frames over time**. Every physical thing on a robot (base, camera, arm, gripper, wheel, world, map) has its own coordinate frame. TF2 lets you ask questions like:

> *"Where is the camera relative to the world right now?"*
> *"What's the position of this LIDAR point in the robot's base frame?"*

TF2 stores a **tree of transforms** (each frame has exactly one parent) and a recent **history**, so you can also ask "where was the camera 0.2 seconds ago?"

```
        world
          │
          ├── sensor_static   (fixed offset, never moves)
          └── robot           (moves over time)
                 ├── lidar
                 └── camera
```

## The three roles

| Role | Class | What it does |
|------|-------|-------------|
| Static broadcaster | `StaticTransformBroadcaster` | Publishes a transform once. Used for things that never move (sensor mounting offsets). |
| Dynamic broadcaster | `TransformBroadcaster` | Publishes a changing transform many times per second. Used for robot motion. |
| Listener | `TransformListener` + `Buffer` | Queries the TF tree for any frame-to-frame transform. |

## What's in this package

| File | Role |
|------|------|
| `src/static_broadcaster.cpp` | Publishes `world -> sensor_static` (constant offset) |
| `src/dynamic_broadcaster.cpp` | Publishes `world -> robot` (moving in a circle, 20 Hz) |
| `src/listener.cpp` | Looks up `world -> robot` every 0.5 s and prints x, y, yaw |
| `launch/tf_demo.launch.py` | Starts all three at once |

## Build & run

```bash
colcon build --packages-select pkg10_tf2_cpp
source install/setup.bash

# Easy way: launch all three together
ros2 launch pkg10_tf2_cpp tf_demo.launch.py
```

Or one at a time in separate terminals:
```bash
ros2 run pkg10_tf2_cpp static_broadcaster
ros2 run pkg10_tf2_cpp dynamic_broadcaster
ros2 run pkg10_tf2_cpp listener
```

## Visualize the TF tree

```bash
# Print the current frames and their relationships
ros2 run tf2_ros tf2_echo world robot

# Generate a PDF diagram of the tree
ros2 run tf2_tools view_frames
# (creates frames.pdf in current directory)

# Or use rviz2 — add a TF display and set Fixed Frame to "world"
rviz2
```

## Quaternions, briefly

ROS represents rotations as **quaternions** (x, y, z, w), not Euler angles, because Euler angles have singularities (gimbal lock). To make one from yaw/pitch/roll:

```cpp
#include "tf2/LinearMath/Quaternion.h"
tf2::Quaternion q;
q.setRPY(roll, pitch, yaw);   // radians
```

To convert back:
```cpp
#include "tf2/utils.h"
double yaw = tf2::getYaw(transform.rotation);
```

## ⚠️ Common pitfalls

1. **Forgetting to publish at high enough rate.** TF buffers a few seconds of history. If you publish at 1 Hz and ask for "now", you may miss the window. Use ≥ 20 Hz for moving frames.
2. **`tf2::TimePointZero` means "latest available"**, not "at time zero". Use it unless you specifically need a past time.
3. **The TF tree must be acyclic and connected.** If two different nodes publish `world -> robot`, you get a fight.
4. **Frame IDs are case-sensitive strings.** `Robot` and `robot` are different frames.

## Exercise

1. Modify the dynamic broadcaster to also publish `robot -> camera` at offset `(0.2, 0, 0.3)`. Now the listener can ask for `world -> camera` — try it.
2. Use `tf2_echo` to verify the chain.
