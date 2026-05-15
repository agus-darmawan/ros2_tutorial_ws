# 03 — Custom Interfaces (msg / srv / action)

ROS2 ships with common messages (`std_msgs`, `geometry_msgs`, `sensor_msgs`, ...). When the built-in ones don't fit your data, you write your own **interface files**.

## Three kinds of interface

| Kind | File extension | Sections | Used by |
|------|---------------|----------|---------|
| Message | `.msg` | 1 (just fields) | Topics |
| Service | `.srv` | 2 (request `---` response) | Services |
| Action | `.action` | 3 (goal `---` result `---` feedback) | Actions |

## Files in this package

```
msg/Temperature.msg          # custom topic message
srv/AddTwoInts.srv           # request/response example
srv/SetLedColor.srv          # request/response with status
action/CountUntil.action     # long-running task with feedback
```

## Field types you can use

```
bool, byte, char
float32, float64
int8, int16, int32, int64
uint8, uint16, uint32, uint64
string
<another_pkg>/<MsgType>      # e.g. geometry_msgs/Pose
<MsgType>[]                  # dynamic array
<MsgType>[N]                 # fixed-size array
```

You can also define **constants** with capital names:

```
uint8 STATUS_OK = 0
uint8 STATUS_ERROR = 1
uint8 status
```

## Build

```bash
colcon build --packages-select pkg03_tutorial_interfaces
source install/setup.bash
```

## Verify the interfaces were generated

```bash
ros2 interface show pkg03_tutorial_interfaces/msg/Temperature
ros2 interface show pkg03_tutorial_interfaces/srv/AddTwoInts
ros2 interface show pkg03_tutorial_interfaces/action/CountUntil

ros2 interface list | grep pkg03_tutorial_interfaces
```

## How other packages use these

In another **C++** package's `package.xml`:
```xml
<depend>pkg03_tutorial_interfaces</depend>
```

In `CMakeLists.txt`:
```cmake
find_package(pkg03_tutorial_interfaces REQUIRED)
ament_target_dependencies(my_node pkg03_tutorial_interfaces)
```

In the code:
```cpp
#include "pkg03_tutorial_interfaces/srv/add_two_ints.hpp"
// note: CamelCase -> snake_case for header names
```

In **Python**:
```python
from pkg03_tutorial_interfaces.srv import AddTwoInts
```

## Naming convention

- **Files / type names:** `CamelCase` (e.g. `Temperature.msg`, `AddTwoInts.srv`)
- **Field names:** `snake_case` (e.g. `sensor_name`, `temperature`)
- **Constants:** `UPPER_SNAKE_CASE`
