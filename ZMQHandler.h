//
// Created by lgarcia on 18/02/16.
//

#ifndef ACMR_CONTROL_ZMQHANDLER_H
#define ACMR_CONTROL_ZMQHANDLER_H

#include <zmq.h>

class ZMQHandler {
    ZMQHandler();
    void run();
    void read();
    void write();


private:
    bool exitFlag;
};


#endif //ACMR_CONTROL_ZMQHANDLER_H
