//
//  HttpServer.hpp
//  jewginx
//
//  Created by Alexander Rogovsky on 18/02/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#ifndef HttpServer_hpp
#define HttpServer_hpp

#include <event2/event.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <stdio.h>

class HttpServer{
public:
    HttpServer();
    void start(int port, int workers);
    
private:
    static void connectionCallback(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ptr);
    
    static void errorCallback(struct evconnlistener *listener, void *ctx);
    static void readCallback(struct bufferevent *bev, void *ctx);
    static void eventCallback(struct bufferevent *bev, short events, void *ctx);
    static void writeCallback(bufferevent *bev, void *ctx);

};

#endif /* HttpServer_hpp */
