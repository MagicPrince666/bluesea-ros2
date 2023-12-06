#ifndef __DATA_PROC__
#define __DATA_PROC__

#include<vector>
#include"parser.h"
#if defined(USE_ROS_NORTIC_VERSION) || defined(USE_ROS_MELODIC_VERSION)
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
using LaserScanMsg = sensor_msgs::LaserScan;
#else
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
using LaserScanMsg = sensor_msgs::msg::LaserScan;
#endif

typedef struct
{
    unsigned short N;
    uint32_t ts[2];

    struct DataPoint points[0];

}DataPoints,*PDataPoints;

bool data_process(LaserScanMsg&);
bool whole_data_process(RawData raw, bool from_zero,int collect_angle,std::vector<RawData>& whole_datas,PDataPoints *data,int &size,char *result);
int autoGetFirstAngle(RawData raw, bool from_zero, std::vector<RawData> &raws,std::string &result);
int find(std::vector<RawData>a, int n, int x);
#endif