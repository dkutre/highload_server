//
// Created by ovv on 13.05.17.
//

#include "Connection.h"

Connection::Connection(boost::asio::io_service& service, RequestHandler& handler):
   strand_(service),
   socket_(service),
   requestHandler_(handler)
{}

boost::asio::ip::tcp::socket& Connection::getSocket()
{
    return socket_;
}

void Connection::start()
{
    auto shared_this = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_),
                            strand_.wrap([shared_this](const boost::system::error_code& e,
                                                       std::size_t bytes_transferred) {
                                shared_this->handleRead(e, bytes_transferred);
                            }));
}

void Connection::handleRead(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    if (!e) {
        boost::tribool result;
        boost::tie(result, boost::tuples::ignore) = requestParser_.parse(
                request_, buffer_.data(), buffer_.data() + bytes_transferred);

        auto shared_this = shared_from_this();
        if (result) {
            requestHandler_.handleRequest(request_, reply_);
            boost::asio::async_write(socket_, reply_.toBuffers(),
                                     strand_.wrap([shared_this](const boost::system::error_code &e, size_t bytes) {
                                         shared_this->handleWrite(e);
                                     }));

        } else if (!result) {
            reply_ = Reply::getDefaulReply(Reply::StatusType::bad_request);
            boost::asio::async_write(socket_, reply_.toBuffers(),
                                     strand_.wrap([shared_this](const boost::system::error_code &e, size_t bytes) {
                                         shared_this->handleWrite(e);
                                     }));
        } else {
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    strand_.wrap([shared_this](const boost::system::error_code &e, size_t bytes) {
                                        shared_this->handleRead(e, bytes);
                                    }));
        }
    }
}

void Connection::handleWrite(const boost::system::error_code &e)
{
    if (!e) {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }
}