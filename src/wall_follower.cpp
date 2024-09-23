#include <iostream>
#include <cmath>

#include <signal.h>

#include <mbot_bridge/robot.h>
#include <wall_follower/common/utils.h>
#include <cmath>

bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    // Initialize the robot.
    mbot_bridge::MBot robot;
    // Create empty vectors to store the scan data.
    std::vector<float> ranges;
    std::vector<float> thetas;

    /**
     * TODO: Declare any variables you need here.
     */
    float setpoint = 0.45;
    float v_to_wall = 0.4;
    float error_tolerance = 0.2;
    float proportional_constant = 0.15;
    std::vector<float> perp_vector = {0.0, 0.0, 1.0};

    while (true)
    {
        // This function gets the Lidar scan data.
        robot.readLidarScan(ranges, thetas);

        float min_idx = findMinDist(ranges);
        if (min_idx == -1)
        {
            continue;
        }
        float dist_to_wall = ranges[min_idx];
        float angle_to_wall = thetas[min_idx];

        //     /**
        //      * TODO: (P1.2) Write code to follow the nearest wall here.
        //      *
        //      * HINT: You should use the functions crossProduct and findMinDist.
        //      */

        std::vector<float> vector_to_wall = {cos((double)angle_to_wall) * v_to_wall, sin((double)angle_to_wall) * v_to_wall, 0};
        std::vector<float> vector_along_wall = crossProduct(vector_to_wall, perp_vector);

        float err = setpoint - dist_to_wall;
        float cur_vel = proportional_constant * err;

        std::vector<float> correction_vector = {cur_vel * cos(angle_to_wall), cur_vel * sin(angle_to_wall), 0};

        float final_x_along = vector_along_wall[0] - correction_vector[0];
        float final_y_along = vector_along_wall[1] - correction_vector[1];

        robot.drive(std::ceil((final_x_along * 100.0)) / 100.0, std::ceil((final_y_along * 100.0)) / 100.0, 0);

        if (ctrl_c_pressed)
            break;
    }

    // Stop the robot.
    robot.stop();
    return 0;
}
