//
//  Constants.hpp
//  jewginx
//
//  Created by Alexander Rogovsky on 08/03/16.
//  Copyright © 2016 leet software inc. All rights reserved.
//

#ifndef Constants_hpp
#define Constants_hpp

#include <stdio.h>
#include <cstdlib>
#include <map>
#include <string>

// Default parameters
extern char* ROOT_DIR;
extern const char* DEFAULT_ROOT_DIR;
extern const int WORKERS_COUNT;
extern const int DEFAULT_PORT;
extern const int MAX_HTTP_REQUEST;
extern const char* INDEX_FILE;

// HTTP responses
extern const char* METHOD_NOT_ALLOWED;
extern const char* BAD_REQUEST;
extern const char* OK;
extern const char* NOT_FOUND;

extern std::map<std::string, const char*> EXTENSIONS;
#endif /* Constants_hpp */
