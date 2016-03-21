#include <iostream>
#include "zmq.hpp"
#include <unistd.h>
#include "ZMQHandler.h"
#include <sstream>
#include "general_defs.h"
#include "PID.h"
#include <math.h>

using namespace std;


/*
 * Takes the propulsion scheme and param, returns the  corresponding desired tailwing angle
 */
int adjustTailWing(int scheme, int parameter, int awa)
{
    int tailwing = 0;
    int windsign = (awa >= 0 ? 1 : -1); // 1 means awa > 0, i.e. wind is from port side, -1 otherwise

    // implements the polar diagram for the tailwing setting from Elkaim's paper, see ENPH 459 Project Proposal, Section 16.6.2 for details
    if(abs(awa) > DONT_GO_WING_ANGLE)
    {
        tailwing = (int)((windsign)*(parameter/100)*MAX_DEFLECTION*(180 - abs(awa))/(180.0-DONT_GO_WING_ANGLE)); // scale as the boat goes through the don't go zone
    }
    else if(abs(awa) > NO_GO_WIND_ANGLE)
    {
        tailwing = windsign*(parameter/100)*MAX_DEFLECTION;
    }
    else if(abs(awa) > UPWIND_HYSTERESIS_ANGLE)
    {
        tailwing = (int)((windsign)*(parameter/100)*MAX_DEFLECTION*(abs(awa)-UPWIND_HYSTERESIS_ANGLE)/(float(NO_GO_WIND_ANGLE-UPWIND_HYSTERESIS_ANGLE)));
    }
    else
    {
        tailwing = 0; // keep neutral for zero thrust
    }

//    switch(scheme)
//    {
//        case ThrustPower: {
//
//            break;
//        }
//        case PropulsionPower: {
////            error = setpoint - AWA;
//            break;
//        }
//        case FixedSpeed: {
////            error = setpoint - HOG;
//            break;
//        }
//    }

    return tailwing;
}

int main() {

    char filter[] = "PROP_S AW";
    ZMQHandler handler(filter);

    int scheme = 0;
    double dt;
    double dt_sum = 0;
    double AWS, AWA, setpoint = 0;

    int tailangle; // angle of the tail wing
//
    clock_t tick = clock();
    clock_t tick_2 = clock();


    while(true)
    {
        double secsElapsed = double(clock() - tick_2) / CLOCKS_PER_SEC;

        bool printed = false;
        std::string rec_str = handler.read();

        if(rec_str.size() > 0) {

            stringstream recstream(rec_str);

            std::string topic;
            recstream >> topic; // read the first string, which is the topic

            if (topic == "PROP_S") {
                int i;

                recstream >> i;

                scheme = i; // the scheme to use for steering

                recstream >> setpoint;
            }
            else if (topic == "AW") {
                recstream >> AWS;
                recstream >> AWA;
//                std::cout << "AWS: " << AWS << "AWA: " << AWA << std::endl;
            }
        }

        tailangle = adjustTailWing(scheme, setpoint, AWA);


        if(secsElapsed > PRINT_DELAY) {

//            cout << "Received: " << rec_str;
//            std::cout << "Scheme: " << scheme << " Setpoint: " << setpoint << std::endl;
            if(scheme == ThrustPower)
                std::cout << "Thrust %: " << setpoint << ", Tail Angle:" << tailangle;
            printed = true;
        }

        dt = double(clock() - tick) / CLOCKS_PER_SEC;

        tick = clock();

        dt_sum += dt;

        if(dt_sum > TCU_PUBLISH_PERIOD)
        {
            std::string send_str = "PROP " + std::to_string(tailangle);
            handler.write(send_str);
            dt_sum = 0;
        }

        if(printed)
        {
            tick_2 = clock();
            cout << std::endl;
        }



        // Redundant lines, I think
        // Need to continuously compute error but send data at a slower rate
//        if( dt_sum > 0.1 ) {
//
//            dt_sum = 0;
//            std::string send_str = "PROP " + std::to_string(tailangle);
//            cout << tailangle << endl;
//
//            handler.write(send_str);
//        }
//
//        if(printed)
//        {
//            tick_2 = clock();
//            std::cout << std::endl;
//        }
    }

    return 0;

}


