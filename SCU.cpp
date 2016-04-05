#include <iostream>
#include "zmq.hpp"
#include <unistd.h>
#include "ZMQHandler.h"
#include <sstream>
#include "general_defs.h"
#include "PID.h"
#include "ctime"

using namespace std;

int main() {

    char filter[] = "STEER_S GPS AW";
    ZMQHandler handler(filter);

    int scheme = 0;

    double setpoint = 0;
    double error = 0;
    double dt;
    double dt_sum = 0;
    double COG = 0;
    double HOG = 0;
    double AWS, AWA;
    clock_t tick = clock();
    clock_t tick_2 = clock();

    // TODO fix magic numbers
    PID pid(1, 0, 0, 10, 45);


    while(true)
    {
        double secsElapsed = double(clock() - tick_2) / CLOCKS_PER_SEC;
//        std::cout << secsElapsed << std::endl;
        bool printed = false;
        if(secsElapsed > PRINT_DELAY) {
            std::cout << "Scheme: " << scheme << " Setpoint: " << setpoint;
            printed = true;
        }
        std::string rec_str = handler.read();

        if(rec_str.size() > 0) {
            stringstream recstream(rec_str);
            std::string topic;
            recstream >> topic;

            if (topic == "STEER_S") {
                int i;
                recstream >> i;
                scheme = i;
                recstream >> setpoint;
            }
            else if (topic == "GPS") {
                double i;
                recstream >> i;
                recstream >> i;
                recstream >> HOG;
                recstream >> i;
                recstream >> COG;
            }
            else if (topic == "AW") {
                recstream >> AWS;
                recstream >> AWA;
            }
        }

        switch(scheme)
        {
            case SailByCOG: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by COG";
                }
                if (abs(COG - setpoint) < 180) {
                    error = setpoint - COG;
                }
                else {
                    error = COG - setpoint;
                }
                break;
            }
            case SailByAWA: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by AWA";
                }
                error = AWA - setpoint;
//                if(setpoint < 0){
//                    error = -error;
//                }
                break;
            }
            case SailByHOG: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by HOG";
                }
                error = setpoint - HOG;
                break;
            }
            case SailByRudder: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by Rudder";
                }
                break;
            }
        }

        dt = double(clock() - tick) / CLOCKS_PER_SEC;

        tick = clock();

        pid.Compute(error, dt);

        dt_sum += dt;

        //doStuffWithOutput(pid.output())

        if(secsElapsed > PRINT_DELAY) {
            if(scheme == SailByRudder){
                cout << ", RUD: " << (int) setpoint;
            }
            else {
                cout << ", RUD: " << (int) pid.Output() << ", Error: " << error;
            }
        }

        // Need to continuously compute error but send data at a slower rate
        if( dt_sum > SCU_PUBLISH_PERIOD ) {

            dt_sum = 0;
            std::string send_str;
            if(scheme == SailByRudder){
                send_str = "RUD " + std::to_string((int) setpoint);
            }
            else {
                send_str = "RUD " + std::to_string((int) pid.Output());
            }
            // cout << send_str << endl;

            handler.write(send_str);
        }

        if(printed)
        {
            tick_2 = clock();
            cout << std::endl;
        }

    }

    return 0;
}

