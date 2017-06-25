//
//  HttpResponse.hpp
//  jewginx
//
//  Created by Alexander Rogovsky on 08/03/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#ifndef HttpResponse_hpp
#define HttpResponse_hpp

#include <stdio.h>
#include <event2/buffer.h>
#include "Constants.hpp"

class HttpResponse{
public:
    HttpResponse(int);
    evbuffer* makeResponse();
    const char* getStatusName();
    void addHeader(const char *name, const char *value);
    void addHeader(const char *name, int value);
private:
    int httpStatus;
    evbuffer* rawResponse;
};

#endif /* HttpResponse_hpp */
