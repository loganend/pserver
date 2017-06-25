//
//  HttpRequest.cpp
//  jewginx
//
//  Created by Alexander Rogovsky on 08/03/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#include "HttpRequest.hpp"
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

HttpRequest::HttpRequest(char* raw){
    rawReq = raw;
}

const char* HttpRequest::getContentType(char* extension)
{
    if(extension == NULL) {
        return EXTENSIONS["no_ext"];
    } else {
        const char* ext =  EXTENSIONS[extension];
        if(ext == NULL) {
            return  EXTENSIONS["default"];
        } else {
            return ext;
        }
    }
}

evbuffer* HttpRequest::parseHttp(){
    
    if (!rawReq){
        fprintf(stdout, "ERROR\n");
    }
    
    char* test = rawReq;
    
    char* reqType;
    reqType = strtok(test, " ");
    if(reqType == NULL || (strcmp(reqType, "GET") != 0 && strcmp(reqType, "HEAD") != 0)) {
        return (new HttpResponse(405))->makeResponse();
    }
    
    char* file;
    file = strtok(NULL, " ");
    if(file == NULL) {
        return (new HttpResponse(400))->makeResponse();
    }
    
    char* decoded = (char*) calloc(1000, sizeof(char));
    urldecode(decoded, 1000, file);
    file = decoded;
    
    char* index = strrchr(file, '?');
    if(index != NULL) {
        index[0] = '\0';
    }
    
    char* path = (char *)malloc(1000);
    strcpy(path, ROOT_DIR);
    strcat(path, file);
    
    bool isindex = false;
    if(file[strlen(file) - 1] == '/') {
        strcat(path, const_cast<char*>(INDEX_FILE));
        isindex = true;
    }
    
    if(strstr(path, "/../") != NULL) {
        free(path);
        free(decoded);
        return (new HttpResponse(404))->makeResponse();
    }
    
    char* extension = 0;
    size_t i = strlen(path) - 1;
    while(path[i] != '/') {
        --i;
        if(path[i] == '.') {
            extension = (char*) calloc(strlen(path) - i, sizeof(char));
            strcpy(extension, path + i + 1);
            break;
        }
    }
    
    int fd = open(path, O_RDONLY);
    if(fd < 0) {
        free(path);
        free(decoded);
        if(isindex) {
            return (new HttpResponse(403))->makeResponse();
        } else {
            return (new HttpResponse(404))->makeResponse();
        }
    }
    
    struct stat buf;
    fstat (fd, &buf);

    HttpResponse* response = new HttpResponse(200);
    response->addHeader("Content-type", getContentType(extension));
    response->addHeader("Content-Length", static_cast<int>(buf.st_size));
    evbuffer *temp = response->makeResponse();
    if(strcmp(reqType, "GET") == 0) {
        evbuffer_add_file(temp, fd, 0, buf.st_size);
    }
    
    if(path != NULL)
        free(path);
    if(extension != NULL)
        free(extension);
    
    return temp;
}

void HttpRequest::urldecode(char *pszDecodedOut, size_t nBufferSize, char *pszEncodedIn)
{
    memset(pszDecodedOut, 0, nBufferSize);
    
    enum DecodeState_e
    {
        STATE_SEARCH = 0, ///< searching for an ampersand to convert
        STATE_CONVERTING, ///< convert the two proceeding characters from hex
    };
    
    DecodeState_e state = STATE_SEARCH;
    
    for(unsigned int i = 0; i <= strlen(pszEncodedIn)-1; ++i)
    {
        if(pszEncodedIn[i] == '+') {
            pszEncodedIn[i] = ' ';
        }
        switch(state)
        {
            case STATE_SEARCH:
            {
                if(pszEncodedIn[i] != '%')
                {
                    strncat(pszDecodedOut, &pszEncodedIn[i], 1);
                    assert(strlen(pszDecodedOut) < nBufferSize);
                    break;
                }
                
                // We are now converting
                state = STATE_CONVERTING;
            }
                break;
                
            case STATE_CONVERTING:
            {
                // Conversion complete (i.e. don't convert again next iter)
                state = STATE_SEARCH;
                
                // Create a buffer to hold the hex. For example, if %20, this
                // buffer would hold 20 (in ASCII)
                char pszTempNumBuf[3] = {0};
                strncpy(pszTempNumBuf, &pszEncodedIn[i], 2);
                
                // Ensure both characters are hexadecimal
                bool bBothDigits = true;
                
                for(int j = 0; j < 2; ++j)
                {
                    if(!isxdigit(pszTempNumBuf[j]))
                        bBothDigits = false;
                }
                
                if(!bBothDigits)
                    break;
                
                // Convert two hexadecimal characters into one character
                int nAsciiCharacter;
                sscanf(pszTempNumBuf, "%x", &nAsciiCharacter);
                
                // Ensure we aren't going to overflow
                assert(strlen(pszDecodedOut) < nBufferSize);
                
                // Concatenate this character onto the output
                strncat(pszDecodedOut, (char*)&nAsciiCharacter, 1);
                
                // Skip the next character
                i++;
            }
                break;
        }
    }
}

