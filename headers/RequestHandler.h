//
// Created by ovv on 13.05.17.
//

#ifndef HIGHLOAD_SERVER_REQUEST_HANDLER_H
#define HIGHLOAD_SERVER_REQUEST_HANDLER_H

#include <string>
#include <boost/noncopyable.hpp>

struct Reply;
struct Request;

class RequestHandler : private boost::noncopyable
{
public:
    /// Construct with a directory containing files to be served.
    explicit RequestHandler(const std::string& rootDirectory);

    /// Handle a request and produce a reply.
    void handleRequest(const Request &req, Reply &rep);

private:
    /// The directory containing the files to be served.
    std::string rootDirectory;

    /// delete get params from request_path
    void deleteGetParams(std::string& path);

    /// Perform URL-decoding on a string. Returns false if the encoding was
    /// invalid.
    static bool urlDecode(const std::string& in, std::string& out);
};


#endif //HIGHLOAD_SERVER_REQUEST_HANDLER_H
