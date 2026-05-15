# 09 — Parameters in Python

Python twin of `pkg08_parameters_cpp`. Same concept — see that README for the conceptual background.

## Key API (rclpy)

```python
# Declare (required before read)
self.declare_parameter('name', default_value)

# Read
val = self.get_parameter('name').value     # any type
val = self.get_parameter('name').get_parameter_value().string_value  # typed

# Callback on change
self.add_on_set_parameters_callback(self.on_param_change)

def on_param_change(self, params):
    result = SetParametersResult(successful=True)
    for p in params:
        # p.name, p.value, p.type_
        ...
    return result
```

## Build & run

```bash
colcon build --packages-select pkg09_parameters_py
source install/setup.bash

# Defaults
ros2 run pkg09_parameters_py params_node

# With YAML
ros2 run pkg09_parameters_py params_node --ros-args \
  --params-file $(ros2 pkg prefix pkg09_parameters_py)/share/pkg09_parameters_py/config/params.yaml

# Inline override
ros2 run pkg09_parameters_py params_node --ros-args -p max_speed:=42
```

## Try it live

```bash
ros2 param list /params_node_py
ros2 param set /params_node_py max_speed 200   # rejected by callback
ros2 param set /params_node_py max_speed 50    # accepted
```

## Exercise

Add a `bool` parameter `verbose`. When `true`, log a debug message every tick. When `false`, stay silent. Toggle it live with `ros2 param set`.
