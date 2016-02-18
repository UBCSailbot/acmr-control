#include <iostream>
#include "zmq.hpp"
#include <unistd.h>
#include "ZMQHandler.h"
#include <sstream>
#include "schemes.h"
#include "PID.h"
#include "ctime"

using namespace std;

int main() {

    char filter[] = "STEER_S GPS AW";
    ZMQHandler handler(filter);

    int scheme;

    double setpoint = 0;
    double error = 0;
    double dt;

    int AWA, COG, HOG;

    clock_t tick = clock();

    // TODO fix magic numbers
    PID pid(0.2, 0.1, 0.3, 5, 70);

    while(true)
    {
        std::string rec_str = handler.read();
        if(rec_str.size() > 0)
        {
            std::cout << rec_str << std::endl;


            stringstream recstream(rec_str);

            std::string topic;
            recstream >> topic;

            if(topic == "STEER_S")
            {
                int i;

                recstream >> i;

                scheme = i;

                recstream >> setpoint;
            }
            else if (topic == "GPS")
            {
                int i;

                recstream >> i;
                recstream >> i;
                recstream >> HOG;
                recstream >> i;
                recstream >> COG;
            }
            else if (topic == "AW")
            {
                int i;
                recstream >> i;
                recstream >> AWA;
            }
        }

        switch(scheme)
        {
            case SailByCOG:
                error = setpoint - COG;
            case SailByAWA:
                error = setpoint - AWA;
            case SailByHOG:
                error = setpoint - HOG;
        }

        dt = double(clock() - tick) / CLOCKS_PER_SEC;

        tick = clock();

        pid.Compute(error, dt);

//        cout << "error: " << error << endl;

        //doStuffWithOutput(pid.output())

        cout << "RUD: " << (int) pid.Output() << ", Error: " << error << endl;

        std::string send_str = "RUD " + std::to_string((int)pid.Output());

        cout << send_str << endl;

        handler.write(send_str);



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


