# 12 — Launch Files

## Why launch files?

In a real robot you don't run nodes one by one. You'd start a dozen of them at once — driver, navigation, perception, TF, control, monitoring — each with their own parameters, namespaces, and topic remappings. A **launch file** describes that whole graph in one place.

ROS2 launch files are **Python scripts** (not XML). They build a `LaunchDescription` object that the `ros2 launch` command then executes.

## What this package contains

| File | What it shows |
|------|---------------|
| `launch/01_minimal.launch.py` | The smallest useful launch file — start two nodes together |
| `launch/02_remap_and_args.launch.py` | Topic remapping + command-line arguments + parameters |
| `launch/03_namespace_and_include.launch.py` | ROS namespaces + including other launch files |

## Build & run

```bash
colcon build --packages-select pkg12_launch_examples
source install/setup.bash

# Example 1 — start publisher + subscriber together
ros2 launch pkg12_launch_examples 01_minimal.launch.py

# Example 2 — pass an argument from the command line
ros2 launch pkg12_launch_examples 02_remap_and_args.launch.py rate:=5.0

# Example 3 — include another launch file inside a namespace
ros2 launch pkg12_launch_examples 03_namespace_and_include.launch.py
```

In example 3, after launch:
```bash
ros2 topic list      # everything is prefixed with /robot1/...
ros2 run tf2_ros tf2_echo /robot1/world /robot1/robot
```

## Key building blocks

```python
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, GroupAction
from launch.substitutions import LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node, PushRosNamespace

# A node to start
Node(
    package='my_pkg',
    executable='my_node',
    name='renamed_at_runtime',          # override the node name
    namespace='robot1',                 # prefix all topics/services
    parameters=[{'speed': 1.0}],        # or [config_yaml_path]
    remappings=[('chatter', '/foo')],   # rename topics
    output='screen',                    # log to terminal
)

# A user-passable argument
DeclareLaunchArgument('rate', default_value='1.0', description='Loop rate Hz')
LaunchConfiguration('rate')             # use the value somewhere

# Include another launch file
IncludeLaunchDescription(
    PythonLaunchDescriptionSource('/path/to/other.launch.py')
)

# Group a set of actions under a namespace
GroupAction([
    PushRosNamespace('robot1'),
    Node(...),
    Node(...),
])
```

## Tips

- Prefer **one launch file per logical "system"** (e.g. `bringup.launch.py`, `nav.launch.py`, `sim.launch.py`).
- Top-level launch files **include** smaller ones — don't repeat node definitions.
- Use **launch arguments** for anything that might change between runs (sim vs real, robot name, port number).
- For parameters that grow, prefer loading a **YAML file** over inlining the dict.

## Exercise

1. Modify `02_remap_and_args.launch.py` to accept a `robot_name` argument and use it as both the namespace and a parameter on the node.
2. Write a `bringup.launch.py` that starts the TF demo (`pkg10_tf2_cpp`) and the parameters demo (`pkg08_parameters_cpp`) together under namespace `/sim`.
