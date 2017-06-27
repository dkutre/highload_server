//
// Created by ovv on 13.05.17.
//

#ifndef HIGHLOAD_SERVER_REQUEST_H
#define HIGHLOAD_SERVER_REQUEST_H

#include <string>
#include "Header.h"
#include <vector>

struct Request
{
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
};


#endif //HIGHLOAD_SERVER_REQUEST_H
