//
// Created by lgarcia on 18/02/16.
//

#include "zmq.hpp"
#include "ZMQHandler.h"
#define ZMQ_DELAY = 0.1;
#include <iostream>

ZMQHandler::ZMQHandler(char *filter) : context(1), pubsocket(context, ZMQ_PUB), subsocket(context, ZMQ_SUB){

//    context (1);
//    pubsocket (context, ZMQ_PUB);
    pubsocket.connect("tcp://localhost:5551");

//    subsocket (context, ZMQ_SUB);
    subsocket.connect("tcp://localhost:5552");

    char* token = std::strtok(filter, " ");
    while(token != NULL)
    {
        std::cout << token << std::endl;
        subsocket.setsockopt(ZMQ_SUBSCRIBE, token, strlen(token));
        token = std::strtok(NULL, " ");
    }

}


std::string ZMQHandler::read() {
    zmq::message_t msg;

    try {
        int err = subsocket.recv(&msg, ZMQ_NOBLOCK);
        if(err != -1){
            std::string str = std::string(static_cast<char*>(msg.data()), msg.size());
            return str;
        }
        else
        {
            return std::string();
        }
    }
    catch (...)
    {
        return std::string();
    }

}

void ZMQHandler::write(std::string send_str) {
    zmq::message_t msg(send_str.size());

    memcpy(msg.data(), send_str.c_str(), send_str.size());

    pubsocket.send(msg);

//    std::cout << "Sent: " << send_str << std::endl; //TODO remove
}
