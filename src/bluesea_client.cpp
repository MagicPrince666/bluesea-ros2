#if defined(USE_ROS_NORTIC_VERSION) || defined(USE_ROS_MELODIC_VERSION)
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_srvs/Empty.h>
using EmptySrv = std_srvs::Empty;
#else
#include "rclcpp/clock.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time_source.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <std_srvs/srv/empty.hpp>
using EmptySrv = std_srvs::srv::Empty;
#endif
#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    // spdlog::info("Current ROS2 Version is {}",getROS2Version());
    if (argc != 2) {
        spdlog::info("usage: ros1/ros2 run bluesea2  bluesea_node_client stop/start");
        return 1;
    }
#if defined(USE_ROS_NORTIC_VERSION) || defined(USE_ROS_MELODIC_VERSION)
    ros::init(argc, argv, "bluesea_node_client");
    auto node = std::make_shared<ros::NodeHandle>();
#else
    rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("bluesea_node_client");

    char cmd[8]                        = {0};
    strcpy(cmd, argv[1]);
    rclcpp::Client<EmptySrv>::SharedPtr client;
    if (strcmp(cmd, "start") == 0) {
        client = node->create_client<EmptySrv>("start"); // CHANGE
    } else if (strcmp(cmd, "stop") == 0) {
        client = node->create_client<EmptySrv>("stop");
    }

    auto request = std::make_shared<EmptySrv::Request>(); // CHANGE
    while (!client->wait_for_service(std::chrono::seconds(1))) {
        if (!rclcpp::ok()) {
            spdlog::error("Interrupted while waiting for the service. Exiting.");
            return 0;
        }
        spdlog::info("service not available, waiting again...");
    }

    auto result = client->async_send_request(request);

// Wait for the result.
// ros2 rolling   20.04
#ifdef ROS_DISTRO_F
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS)
#elif defined ROS_DISTRO_E
    // ros2 eloquent   18.04 and lower
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
#endif
    {
        spdlog::info("client OK");
    } else {
        spdlog::info("Failed to call ctrl_service "); // CHANGE
    }

    rclcpp::shutdown();
#endif
    return 0;
}
