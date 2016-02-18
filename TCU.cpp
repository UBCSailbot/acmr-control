#include <iostream>
#include "zmq.hpp"
#include <unistd.h>
#include "ZMQHandler.h"

using namespace std;

int main() {

    char filter[] = "PROP_S AW";
    ZMQHandler handler(filter);

    while(true)
    {
        std::string rec_str = handler.read();
        if(rec_str.size() > 0)
        {
            std::cout << rec_str << std::endl;
        }


    }

    return 0;

}

