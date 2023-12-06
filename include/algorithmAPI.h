#ifndef _ALGORITHMAPI_
#define _ALGORITHMAPI_ 
//#include <filters/filter_base.hpp>
//#include <laser_filters/SpeckleFilterConfig.h>
#include<math.h>
#if defined(USE_ROS_NORTIC_VERSION) || defined(USE_ROS_MELODIC_VERSION)
#include <sensor_msgs/LaserScan.h>
using LaserScanMsg = sensor_msgs::LaserScan;
#else
#include <sensor_msgs/msg/laser_scan.hpp>
using LaserScanMsg = sensor_msgs::msg::LaserScan;
#endif

bool checkWindowValid(const LaserScanMsg& scan, size_t idx, size_t window, double max_range_difference);
bool checkWindowValid2(const LaserScanMsg& scan, size_t idx, size_t window, double max_distance);

bool filter(const LaserScanMsg& input_scan, LaserScanMsg& output_scan
,int type,double max_range,double min_range,double max_range_difference,int filter_window);
#endif
