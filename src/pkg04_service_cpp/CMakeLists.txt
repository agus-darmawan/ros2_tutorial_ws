cmake_minimum_required(VERSION 3.8)
project(pkg04_service_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pkg03_tutorial_interfaces REQUIRED)

add_executable(server_node src/server_node.cpp)
ament_target_dependencies(server_node rclcpp pkg03_tutorial_interfaces)

add_executable(client_node src/client_node.cpp)
ament_target_dependencies(client_node rclcpp pkg03_tutorial_interfaces)

install(TARGETS
  server_node
  client_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
