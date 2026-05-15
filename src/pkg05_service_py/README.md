# 05 — Services in Python

Python twin of `pkg04_service_cpp`. Same service (`/add_two_ints`), same interface (`pkg03_tutorial_interfaces/srv/AddTwoInts`).

## Key API (rclpy)

```python
# SERVER
self.srv = self.create_service(SrvType, 'name', self.callback)
# callback signature: (request, response) -> response

# CLIENT
self.client = self.create_client(SrvType, 'name')
self.client.wait_for_service(timeout_sec=1.0)
future = self.client.call_async(request)
rclpy.spin_until_future_complete(self, future)
response = future.result()
```

## Build & run

```bash
colcon build --packages-select pkg03_tutorial_interfaces pkg05_service_py
source install/setup.bash
```

**Terminal 1:**
```bash
ros2 run pkg05_service_py server_node
```

**Terminal 2:**
```bash
ros2 run pkg05_service_py client_node 10 20
```

## Cross-language demo

Run the **C++** server with the **Python** client (or vice versa). It just works.

```bash
ros2 run pkg04_service_cpp server_node    # terminal 1
ros2 run pkg05_service_py client_node 4 6 # terminal 2
```

## Common pitfall

`call_async(...)` returns a **Future**. You must spin to drive it. If you call it from inside another callback, do **not** use `spin_until_future_complete` on the same node — it will deadlock. Instead:

```python
future = self.client.call_async(request)
future.add_done_callback(self.response_handler)
```

## Exercise

Add a `divide_two_ints` service. What should happen on division by zero?
