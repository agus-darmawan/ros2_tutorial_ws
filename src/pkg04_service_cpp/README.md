# 04 — Services in C++

## What is a Service?

A **service** is a synchronous, one-to-one request/response interaction between two nodes.

```
 [client_node]  --(Request)-->  /add_two_ints  --(Response)-->  [client_node]
                                  service
                                     ^
                                     |
                              served by [server_node]
```

Unlike topics, a service has exactly **one server**. A client sends a request, waits, and gets a single response back.

**Use services when:**
- The answer depends on a request (compute something, query state).
- You need to know whether an operation succeeded.
- The work is **fast** (milliseconds). For slow work, use an **Action** instead.

## What's in this package

| File | Role |
|------|------|
| `src/server_node.cpp` | Server: receives `(a, b)`, replies with `sum` |
| `src/client_node.cpp` | Client: sends `(a, b)` and prints the result |

The service type `pkg03_tutorial_interfaces/srv/AddTwoInts` is defined in the `pkg03_tutorial_interfaces` package.

## Key API

```cpp
// SERVER
service_ = create_service<SrvType>("name", callback);
// callback signature: (Request::SharedPtr, Response::SharedPtr) -> void

// CLIENT
client_ = create_client<SrvType>("name");
client_->wait_for_service();          // until ready
auto future = client_->async_send_request(request);
rclcpp::spin_until_future_complete(node, future);
auto response = future.get();
```

## Build & run

```bash
colcon build --packages-select pkg03_tutorial_interfaces pkg04_service_cpp
source install/setup.bash
```

**Terminal 1 — server:**
```bash
ros2 run pkg04_service_cpp server_node
```

**Terminal 2 — client:**
```bash
ros2 run pkg04_service_cpp client_node 7 8
# Result: 15
```

You can also call the service from the CLI without any client program:
```bash
ros2 service call /add_two_ints pkg03_tutorial_interfaces/srv/AddTwoInts "{a: 7, b: 8}"
```

## Inspect with the CLI

```bash
ros2 service list                                          # all services
ros2 service type /add_two_ints                            # what type?
ros2 service call /add_two_ints <type> "{...}"             # call it
ros2 interface show pkg03_tutorial_interfaces/srv/AddTwoInts     # see the definition
```

## ⚠️ Common pitfall: deadlock

**Never call a service from inside another callback synchronously** (using `spin_until_future_complete` on the same node). The callback is blocking the executor; the response will never arrive. Either:
- Call the service with `async_send_request` and a **callback** that handles the response.
- Or put the call in a separate thread / callback group (see `executors_cpp`).

## Exercise

1. Modify the server to refuse negative numbers (set `sum = -1` and log a warning).
2. Write a Python client that calls this C++ server (proves language independence).
