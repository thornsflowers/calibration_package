#include <iostream>
#include <pso.h>
#include <ros/ros.h>
#include <time.h>
#include <fstream>
#include <string.h>
#include <std_msgs/Float32.h>
#include <calibration_package/Particle.h>

struct tm tstruct;
char plot[100];
time_t now;
ros::NodeHandle *n_;
ros::Publisher publish_particles;

float PSO::getScore(std::vector<float> values)
{

    // do something here and return the fitness score
    calibration_package::Particle particle_array;

    for (std::vector <float>::iterator it = values.begin(); it != values.end(); ++it)
        particle_array.param.push_back(*it);
    publish_particles.publish(particle_array);

    /// -----
    /// -----Generate particles
    /// -----Publish particles to calibrate node
    /// -----Launch Bag file
    /// -----Calculate score in calibrate file
    /// -----Wait for message (score) in getScore funciton
    /// -----

    std_msgs::Float32::ConstPtr msg = ros::topic::waitForMessage <std_msgs::Float32> ("/Score", *n_);
    float cost = abs(msg->data);

    return cost;
}

int main(int argc, char **argv)
{
//    now = time(0);
//    tstruct = *localtime(&now);
//    strftime(plot, sizeof(plot), "/home/ankur/Desktop/qt_dev/pso/log/PSO-SVM-%Y-%m-%d-%H-%M-%S", &tstruct);
//    strcat(plot, ".csv");

    ros::init(argc, argv, "optimize_node");
    ROS_INFO("Started Search Optimization Node");
    n_ = new ros::NodeHandle;
    publish_particles = n_->advertise<calibration_package::Particle>("particles", 1, true);

    PSO search;
    std::vector <float> parameters;
    parameters = search.getOptimal();

    for (std::vector <float>::iterator it = parameters.begin(); it != parameters.end(); ++it)
        std::cout << "Optimal Parameters " << *it << std::endl;

    delete n_;
    return 0;
}
