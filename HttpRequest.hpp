//
//  HttpRequest.hpp
//  jewginx
//
//  Created by Alexander Rogovsky on 08/03/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#ifndef HttpRequest_hpp
#define HttpRequest_hpp

#include <stdio.h>
#include <cstring>
#include <event2/buffer.h>
#include "HttpResponse.hpp"

class HttpRequest {
public:
    HttpRequest(char*);
    evbuffer* parseHttp();
    static const char* getContentType(char*);
    void urldecode(char *pszDecodedOut, size_t nBufferSize, char *pszEncodedIn);
private:
    char* rawReq;
    
};

#endif /* HttpRequest_hpp */
