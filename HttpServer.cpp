//
//  HttpServer.cpp
//  jewginx
//
//  Created by Alexander Rogovsky on 18/02/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#include "HttpServer.hpp"
#include "Constants.hpp"
#include "HttpRequest.hpp"
#include <iostream>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

HttpServer::HttpServer(){}

void HttpServer::start(int port, int workers){
    struct event_base *base;
    struct sockaddr_in sin;
    struct evconnlistener *listener;
    
    if(!port){
        port = DEFAULT_PORT;
    }
    
    if(!workers){
        workers = WORKERS_COUNT;
    }
    
    base = event_base_new();
    
    if (!base) {
        throw std::runtime_error("Can't create base");
    }
    
    memset(&sin, 0, sizeof(sin));
    
    // setting IPv4 family addresses and localhost
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    // setting port
    sin.sin_port = htons(port);
    
    /*
     struct evconnlistener *evconnlistener_new_bind(struct event_base *base,
        evconnlistener_cb cb, void *ptr, unsigned flags, int backlog,
        const struct sockaddr *sa, int socklen);
     */
    
    listener = evconnlistener_new_bind(base, connectionCallback, nullptr, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, 1024, (struct sockaddr*) &sin, sizeof(sin));
    
    if(listener == NULL){
        event_base_free(base);
        throw std::runtime_error("Couldn't bind port");
    }
    
    /* Fork mechanism */
    for(int i=0; i<3; i++){
        pid_t pid = fork();
        
        if (pid == 0)
        {
            std::cout<<"child process started. pid: " << getpid() << std::endl;
            event_reinit(base);
            
        } else if (pid > 0) {
            // parent process
            break;
            
        } else {
            // fork failed
            std::cout<<"fork failed!\n";
        }
    }
    
    evconnlistener_set_error_cb(listener, errorCallback);
    event_base_dispatch(base);
    
    event_base_free(base);
    return;
}

void HttpServer::connectionCallback(struct evconnlistener *listener, int fd, struct sockaddr *address, int socklen, void *ptr)
{
    fprintf(stdout, "Got connection! %d \n", getpid());
    /* We got a new connection! Set up a bufferevent for it. */
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    
    bufferevent_setcb(bev, readCallback, writeCallback, eventCallback, NULL);
    bufferevent_enable(bev, EV_READ|EV_PERSIST);
}

void HttpServer::errorCallback(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));
    
    event_base_loopexit(base, NULL);
}

void HttpServer::readCallback(struct bufferevent *bev, void *ctx)
{
    /* This callback is invoked when there is data to read on bev. */
    struct evbuffer *input = bufferevent_get_input(bev);
    struct evbuffer *output = bufferevent_get_output(bev);
    
    char *rawRequest = (char *)calloc(MAX_HTTP_REQUEST, sizeof(char));
    evbuffer_copyout(input, rawRequest, evbuffer_get_length(input));
    
    HttpRequest* request = new HttpRequest(rawRequest);
    evbuffer* response = request->parseHttp();
    
    delete request;
    free(rawRequest);
    
    /* Copy all the data response buffer to the output buffer. */
    evbuffer_add_buffer(output, response);
    evbuffer_free(response);
}

void HttpServer::eventCallback(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR)
        fprintf(stdout, "Error from bufferevent\n");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        fprintf(stdout, "freeing\n");
        bufferevent_free(bev);
    }
}

void HttpServer::writeCallback(bufferevent *bev, void *ctx)
{
    bufferevent_free(bev);
}


