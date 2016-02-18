//
// Created by lgarcia on 18/02/16.
//

#include <zmq.hpp>
#include "ZMQHandler.h"
#define ZMQ_DELAY = 0.1;

ZMQHandler::ZMQHandler() {

    zmq::context_t context (1);
    zmq::socket_t pubsocket (context, ZMQ_REP);
    pubsocket.connect("tcp://localhost:5551");

    zmq::socket_t subsocket (context, ZMQ_SUB);
    subsocket.connect("tcp://localhost:5552");
    // subsocket.setsockopt(ZMQ_SUBSCRIBE, )

}

void ZMQHandler::run(){

}