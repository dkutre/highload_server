//
// Created by ovv on 13.05.17.
//

#ifndef HIGHLOAD_SERVER_SERVER_H
#define HIGHLOAD_SERVER_SERVER_H

#include <boost/asio.hpp>
#include <thread>
#include "Connection.h"
#include "RequestHandler.h"
#include <vector>

class Server
{
public:
    Server(const Server&)             = delete;
    Server& operator= (const Server&) = delete;

    Server(const std::string& address, const std::string& port,
           const std::string& rootDirectory, size_t numberThreads);
    void run();

private:
    void startAccept();

    void doAwaitStop();

    std::size_t threadPoolSize_;

    boost::asio::io_service service_;

    boost::asio::signal_set signals_;

    boost::asio::ip::tcp::acceptor acceptor_;

    connectionPointer newConnection_;

    RequestHandler requestHandler_;

};


#endif //HIGHLOAD_SERVER_SERVER_H
