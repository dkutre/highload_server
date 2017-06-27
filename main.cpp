#include <iostream>
#include "Server.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{
    try {
        std::string rootDir = "";
        std::size_t numberThreads = 0;
        int opt = 0;
        while ((opt = getopt(argc, argv, "r:c:")) != -1) {
            switch (opt) {
                case 'r':
                    rootDir = std::string(optarg);
                    if (!rootDir.empty() && rootDir.back() != '/')
                        rootDir += '/';
                    break;
                case 'c':
                    numberThreads = 2 * atoi(optarg);
                    if (numberThreads <= 0)
                        numberThreads = 1;
                    break;
                default:
                    break;
            }
        }
        std:: cout << "threads: " << numberThreads << " root_dir: " << rootDir << std::endl;
        Server s("127.0.0.1", "8000", rootDir, numberThreads);
        s.run();
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}