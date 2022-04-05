#ifndef SOCKETS_HPP
# define SOCKETS_HPP
# include <iostream>
#include <vector>
#include <map>
#include <iterator>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
# include "Location.hpp"
# include <fcntl.h>

class Sockets {
    public:
        Sockets(int port, std::string host);
        void	start_listen();
        int     getFd() const;

    private:
        int                         _port;
        int							_fd;
		struct sockaddr_in			_addr;
        std::string host;
};
#endif