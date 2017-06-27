//
// Created by ovv on 13.05.17.
//

#ifndef HIGHLOAD_SERVER_CONNECTION_H
#define HIGHLOAD_SERVER_CONNECTION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "Reply.h"
#include "Request.h"
#include "RequestParser.h"
#include "RequestHandler.h"

class RequestHandler;
class Reply;
class RequestParser;
class Request;


class Connection: public boost::enable_shared_from_this<Connection>
{
public:
    Connection(const Connection&)             = delete;
    Connection& operator =(const Connection&) = delete;

    Connection(boost::asio::io_service& service, RequestHandler& handler);
    boost::asio::ip::tcp::socket& getSocket();

    void start();

private:

    void handleRead(const boost::system::error_code &e, std::size_t bytes_transferred);
    void handleWrite(const boost::system::error_code &e);

    boost::asio::io_service::strand strand_;
    boost::asio::ip::tcp::socket socket_;
    boost::array<char, 8192> buffer_;

    RequestHandler& requestHandler_;
    Request request_;
    RequestParser requestParser_;
    Reply reply_;
};

typedef boost::shared_ptr<Connection> connectionPointer;


#endif //HIGHLOAD_SERVER_CONNECTION_H
