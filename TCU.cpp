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


    char filter[] = "PROP_S AW";
    ZMQHandler handler(filter);

    int scheme = 0;
    double setpoint = 0;
    double error = 0;
    double dt;
    double dt_sum = 0;

    int tail, top, bottom;
    double AWA, AWS;

    clock_t tick = clock();
    clock_t tick_2 = clock();
    while(true)
    {
        double secsElapsed = double(clock() - tick_2) / CLOCKS_PER_SEC;

        bool printed = false;
        if(secsElapsed > PRINT_DELAY) {
            std::cout << "Scheme: " << scheme << " Setpoint: " << setpoint;
            printed = true;
        }

        std::string rec_str = handler.read();

        if(rec_str.size() > 0) {

            std::stringstream recstream(rec_str);
            std::string topic;
            recstream >> topic;

            if (topic == "PROP_S") {
                int i;
                recstream >> i;
                scheme = i;
                recstream >> setpoint;
            }

            else if (topic == "AW") {
                recstream >> AWS;
                recstream >> AWA;
                cout << AWA << endl;
            }
        }

        switch(scheme){
            case ThrustPower: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Thrust Power";
                }

                if( AWA > 0 ) {
                    tail = -setpoint;
                }
                else {
                    tail = setpoint;
                }
                break;
            }

            case PropulsionPower: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Propulsion Power";
                }
                break;
            }
            case FixedSpeed: {
                if(secsElapsed > PRINT_DELAY) {
                    std::cout << " - Fixed Speed";
                }
                break;
            }
        }

        dt = double(clock() - tick) / CLOCKS_PER_SEC;

        tick = clock();

        dt_sum += dt;

        if(secsElapsed > PRINT_DELAY) {
//              cout << ", PROP: " << (int) pid.Output() << ", Error: " << error;
            std::cout << "PROP: " << (int) setpoint;
        }

        // Need to continuously compute error but send data at a slower rate
        if( dt_sum > 0.1 ) {

            dt_sum = 0;
            std::string send_str = "PROP " + std::to_string(tail);
            cout << tail << endl;

            handler.write(send_str);
        }

        if(printed)
        {
            tick_2 = clock();
            std::cout << std::endl;
        }
    }

    return 0;

}

