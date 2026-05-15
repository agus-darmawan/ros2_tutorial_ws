cmake_minimum_required(VERSION 3.8)
project(pkg08_parameters_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(params_node src/params_node.cpp)
ament_target_dependencies(params_node rclcpp)

install(TARGETS params_node DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
