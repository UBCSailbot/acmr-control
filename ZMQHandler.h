//
// Created by lgarcia on 18/02/16.
//

#ifndef ACMR_CONTROL_ZMQHANDLER_H
#define ACMR_CONTROL_ZMQHANDLER_H

#include <zmq.h>
#include <string>

class ZMQHandler {

public:
    ZMQHandler(char *filter);
    std::string read();
    void write(std::string send_str);


private:
    zmq::context_t context;
    zmq::socket_t pubsocket, subsocket;
};


#endif //ACMR_CONTROL_ZMQHANDLER_H
