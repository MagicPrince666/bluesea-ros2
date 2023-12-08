cmake_minimum_required(VERSION 3.8)
project(bluesea2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Get ROS2 version information (initial letter) from environment variables
string(SUBSTRING $ENV{ROS_DISTRO} 0 1 tmp)
#MESSAGE("ROS_DISTRO2:${tmp}")

string(COMPARE LESS_EQUAL ${tmp} "e" flag)
#MESSAGE("flag:${flag}")


if( ${flag} STREQUAL 1)
add_definitions(-DROS_DISTRO_E)
else()
add_definitions(-DROS_DISTRO_F)
endif()

message(STATUS "${LIB_NAME} Ros version : $ENV{ROS_DISTRO}")
if ($ENV{ROS_DISTRO} MATCHES "iron")
  add_definitions(-DUSE_IRON_VERSION)
  set(USE_ROS1_MIDDLE_PRICE OFF)
elseif ($ENV{ROS_DISTRO} MATCHES "humble")
  add_definitions(-DUSE_HUMBLE_VERSION)
  set(USE_ROS1_MIDDLE_PRICE OFF)
elseif ($ENV{ROS_DISTRO} MATCHES "foxy")
  add_definitions(-DUSE_FOXY_VERSION)
  set(USE_ROS1_MIDDLE_PRICE OFF)
elseif ($ENV{ROS_DISTRO} MATCHES "dashing")
  add_definitions(-DUSE_DASHING_VERSION)
  set(USE_ROS1_MIDDLE_PRICE OFF)
elseif ($ENV{ROS_DISTRO} MATCHES "noetic")
  add_definitions(-DUSE_ROS_NORTIC_VERSION)
  set(USE_ROS1_MIDDLE_PRICE ON)
elseif ($ENV{ROS_DISTRO} MATCHES "melodic")
  add_definitions(-DUSE_ROS_MELODIC_VERSION)
  set(USE_ROS1_MIDDLE_PRICE ON)
endif()

find_package(spdlog REQUIRED)
find_package(rmw REQUIRED)

include_directories(
  ${sensor_msgs_INCLUDE_DIRS}
  include
)

file(GLOB main_srcs 
  src/bluesea_node.cpp
  src/parser.cpp
  src/uart_reader.cpp
  src/udp_reader.cpp
  src/tcp_reader.cpp
  src/data_process.cpp
  src/algorithmAPI.cpp
  src/uart.c
)

if(USE_ROS1_MIDDLE_PRICE)
######################## 使用ROS1 作为中间件 ###########################
find_package(catkin REQUIRED COMPONENTS
  rosconsole
  roscpp
  sensor_msgs
  std_srvs
)

catkin_package(
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(bluesea_node ${main_srcs})
target_link_libraries(bluesea_node ${catkin_LIBRARIES} spdlog::spdlog)

add_executable(bluesea_node_client src/bluesea_client.cpp)
target_link_libraries(bluesea_node_client ${catkin_LIBRARIES} spdlog::spdlog)

install(TARGETS bluesea_node bluesea_node_client
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
#######################################################################
else()
######################## 使用ROS2 作为中间件 ###########################
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_srvs REQUIRED)

add_executable(bluesea_node ${main_srcs})
target_link_libraries(bluesea_node spdlog::spdlog)
ament_target_dependencies(bluesea_node
  "rclcpp"
  "sensor_msgs"
  "visualization_msgs"
  "geometry_msgs"
  "std_srvs"
)
add_executable(bluesea_node_client src/bluesea_client.cpp)
target_link_libraries(bluesea_node_client spdlog::spdlog)
ament_target_dependencies(bluesea_node_client rclcpp std_srvs)

target_include_directories(bluesea_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_compile_features(bluesea_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_compile_features(bluesea_node_client PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS bluesea_node bluesea_node_client
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch params 
  DESTINATION share/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
#######################################################################
endif()
