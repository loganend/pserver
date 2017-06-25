//
//  main.cpp
//  jewginx
//
//  Created by Alexander Rogovsky on 17/02/16.
//  Copyright © 2016 Alexander Rogovsky. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "HttpServer.hpp"
#include "Constants.hpp"

int main(int argc, char** argv)
{
    int port = 0;
    int workers = 2;

    int opt = 0;
    while ( (opt = getopt(argc,argv,"p:w:r:")) != -1){
        switch (opt){
            case 'p':
                port = std::stoi(optarg);
                break;
            case 'w':
                workers = std::stoi(optarg);
                break;
            case 'r':
                ROOT_DIR = const_cast<char*>(optarg);
                break;
        }
    }

    if(!ROOT_DIR){
        ROOT_DIR = const_cast<char*>(DEFAULT_ROOT_DIR);
    }

    HttpServer* server = new HttpServer();
    server->start(port, workers);

    return 0;

}
