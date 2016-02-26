#include <iostream>
#include "zmq.hpp"
#include <unistd.h>
#include "ZMQHandler.h"
#include <sstream>
#include "schemes.h"
#include "PID.h"
#include "ctime"

using namespace std;

#define PRINT_DELAY (0.5)

int main() {

    char filter[] = "STEER_S GPS AW";
    ZMQHandler handler(filter);

    int scheme = 0;

    double setpoint = 0;
    double error = 0;
    double dt;
    double dt_sum = 0;
    int AWA, COG, HOG;

    clock_t tick = clock();
    clock_t tick_2 = clock();

    // TODO fix magic numbers
    PID pid(0.2, 0.1, 0.3, 5, 70);

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

//        std::cout << rec_str << std::endl;
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
                int i;

                recstream >> i;
                recstream >> i;
                recstream >> HOG;
                recstream >> i;
                recstream >> COG;
            }
            else if (topic == "AW") {
                int i;
                recstream >> i;
                recstream >> AWA;
            }
        }

        switch(scheme)
        {
            case SailByCOG: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by COG";
                }
                error = setpoint - COG;
                break;
            }
            case SailByAWA: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by AWA";
                }
                error = setpoint - AWA;
                break;
            }
            case SailByHOG: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Sailing by HOG";
                }
                error = setpoint - HOG;
                break;
            }
        }

        dt = double(clock() - tick) / CLOCKS_PER_SEC;

        tick = clock();

        pid.Compute(error, dt);

        dt_sum += dt;

        //doStuffWithOutput(pid.output())

        if(secsElapsed > PRINT_DELAY) {
            cout << ", RUD: " << (int) pid.Output() << ", Error: " << error;
        }

        // Need to continuously compute error but send data at a slower rate
        if( dt_sum > 0.1 ) {

            dt_sum = 0;



            std::string send_str = "RUD " + std::to_string((int)pid.Output());
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



//int parse(std::string rec_str)
//{
//    stringstream recstream(rec_str);
//
//    std::string topic;
//    recstream >> topic;
//
//    int scheme;
//
//    recstream >> scheme;
//
//    int error = 0;
//
//    return error;
//}



