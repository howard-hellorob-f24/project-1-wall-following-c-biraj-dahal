#include <iostream>
#include <cmath>
#include <csignal>

#include <mbot_bridge/robot.h>
#include <wall_follower/common/utils.h>

bool ctrl_c_pressed = false;

void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, ctrlc);
    signal(SIGTERM, ctrlc);

    mbot_bridge::MBot robot;

    float fwd_velocity = 0.5;
    float rotation_angle = M_PI / 2;
    float angular_velocity = M_PI / 2;
    float rotation_time = rotation_angle / angular_velocity;
    int num_squares = 1;

    float time_for_each_segment = 1.;
    float increment_angle = M_PI / 2;
    float cur_angle = 0;

    for (int i = 0; i < 2; i++)
    {

        for (int i = 0; i < 4; i++)
        {
            robot.drive(fwd_velocity, 0, 0);
            sleepFor(time_for_each_segment);

            robot.stop();
            sleepFor(0.5);

            robot.drive(0, 0, M_PI);
            sleepFor(0.5);

            robot.stop();
            sleepFor(1);

            if (ctrl_c_pressed)
            {
                break;
            }
        }
        if (ctrl_c_pressed)
        {
            break;
        }
    }

    std::cout << "Stopping the robot!!" << std::endl;
    robot.stop();
    return 0;
}
