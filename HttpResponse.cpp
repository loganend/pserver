//
//  HttpResponse.cpp
//  jewginx
//
//  Created by Alexander Rogovsky on 08/03/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#include "HttpResponse.hpp"

HttpResponse::HttpResponse(int code)
{
    httpStatus = code;
    rawResponse = evbuffer_new();
    evbuffer_add_printf(rawResponse, "HTTP/1.1 %d %s\r\n", httpStatus, getStatusName());
    addHeader("Server", "jewginx/0.1");
}

const char* HttpResponse::getStatusName()
{
    switch(httpStatus){
        case 405:
            return METHOD_NOT_ALLOWED;
        case 400:
            return BAD_REQUEST;
        case 404:
            return NOT_FOUND;
        case 200:
            return OK;
        default:
            return OK;
    }
}

void HttpResponse::addHeader(const char *name, const char *value)
{
    evbuffer_add_printf(rawResponse, "%s: %s\r\n", name, value);
}

void HttpResponse::addHeader(const char *name, int value)
{
    evbuffer_add_printf(rawResponse, "%s: %d\r\n", name, value);
}

evbuffer* HttpResponse::makeResponse(){
    //form date value
    time_t rawtime;
    struct tm * ptm;
    time(&rawtime);
    ptm = gmtime(&rawtime);
    char date[80];
    strftime (date, 80, "%a, %d %b %Y %H:%M:%S %Z", ptm);
    addHeader("Date", date);
    addHeader("Connection", "Close");
    
    evbuffer_add_printf(rawResponse, "\r\n");
    
    return rawResponse;
}
