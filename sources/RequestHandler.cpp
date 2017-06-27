//
// Created by ovv on 13.05.17.
//

#include "RequestHandler.h"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "MIME_TYPES.h"
#include "Reply.h"
#include "Request.h"


RequestHandler::RequestHandler(const std::string &rootDirectory)
        : rootDirectory(rootDirectory) {
}

void RequestHandler::handleRequest(const Request &req, Reply &rep) {
    // Decode url to path.
    std::string request_path;
    if (!urlDecode(req.uri, request_path)) {
        rep = Reply::getDefaulReply(Reply::StatusType::bad_request);
        return;
    }
    deleteGetParams(request_path);

    // only get request
    if (req.method == "POST") {
        rep = Reply::getDefaulReply(Reply::StatusType::bad_request);
        return;
    }

    // Request path must be absolute and not contain "..".
    if (request_path.empty() || request_path[0] != '/'
        || request_path.find("/..") != std::string::npos) {
        rep = Reply::getDefaulReply(Reply::StatusType::bad_request);
        return;
    }

    // If path ends in slash (i.e. is a directory) then add "index.html".
    if (request_path[request_path.size() - 1] == '/') {
        request_path += "index.html";
    }

    // Determine the file extension.
    std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t last_dot_pos = request_path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
        extension = request_path.substr(last_dot_pos + 1);
    }

    // Open the file to send back.
    std::string full_path = rootDirectory + request_path;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
        if (request_path.find("index.html") != std::string::npos) {
            rep = Reply::getDefaulReply(Reply::StatusType::forbidden);
        } else {
            rep = Reply::getDefaulReply(Reply::StatusType::not_found);
        }
        return;
    }

    // Fill out the Reply to be sent to the client.
    rep.status = Reply::StatusType::ok;
    char buf[1024];
    while (is.read(buf, sizeof(buf)).gcount() > 0)
        rep.content.append(buf, is.gcount());
    rep.headers.resize(3);
    rep.headers[0].name  = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());

    if (req.method == "HEAD") {
        rep.content.clear();
    }

    rep.headers[1].name  = "Content-Type";
    rep.headers[1].value = transformExtensionToType(extension);
    rep.headers[2].name  = "Server";
    rep.headers[2].value = "mycpp_server";
}

bool RequestHandler::urlDecode(const std::string &in, std::string &out) {
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return true;
}

void RequestHandler::deleteGetParams(std::string &path)
{
    std::size_t last_dot_pos = path.find_last_of(".");
    std::size_t last_question_symbol_pos = path.find_last_of("?");
    if (last_question_symbol_pos != std::string::npos && last_question_symbol_pos > last_dot_pos) {
        path.erase(last_question_symbol_pos);
    }
}
