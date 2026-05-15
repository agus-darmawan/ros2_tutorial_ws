# 08 — Parameters in C++

## What are Parameters?

A **parameter** is a typed, named value attached to a single node. They are used for **configuration without recompiling** — speeds, gains, frame names, file paths, feature flags.

Each node has its own parameter set. Parameters are accessed via the node's name (e.g. `/params_node/max_speed`).

## Typed parameters

ROS2 parameters are **strongly typed**: `bool, int, double, string, byte_array, bool_array, int_array, double_array, string_array`. Set the wrong type and the call is rejected.

## What this package shows

`src/params_node.cpp` demonstrates:
1. **Declaring** parameters with default values.
2. **Reading** them at startup.
3. **Validating** changes at runtime via `add_on_set_parameters_callback`.
4. **Loading** values from a YAML file (`config/params.yaml`).

## Build & run

```bash
colcon build --packages-select pkg08_parameters_cpp
source install/setup.bash

# Defaults
ros2 run pkg08_parameters_cpp params_node

# With YAML
ros2 run pkg08_parameters_cpp params_node \
  --ros-args --params-file $(ros2 pkg prefix pkg08_parameters_cpp)/share/pkg08_parameters_cpp/config/params.yaml

# Override one param on the command line
ros2 run pkg08_parameters_cpp params_node --ros-args -p max_speed:=50
```

## CLI commands you must remember

```bash
ros2 param list                                # all parameters on all nodes
ros2 param list /params_node                   # just this node
ros2 param get /params_node max_speed          # read a value
ros2 param set /params_node max_speed 42       # write a value
ros2 param describe /params_node max_speed     # type + description
ros2 param dump /params_node                   # all params as YAML
```

Watch the runtime callback in action:
```bash
ros2 param set /params_node max_speed 200
# -> server rejects, you see the WARN line in the node's terminal
```

## Best practice

- **Always** call `declare_parameter` for every parameter you read. Undeclared reads throw.
- Group related params under your node and document them in the README.
- For values that may change while running (gains, debug flags), use a `parameter callback` to react.

## Exercise

Add a `string` parameter `mode` that accepts only `"auto"`, `"manual"`, or `"idle"`. Reject any other value in the callback.
