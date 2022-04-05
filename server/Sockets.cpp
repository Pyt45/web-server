#include "Sockets.hpp"

Sockets::Sockets(int port, std::string host) {
    this->_port = port;
    this->host = host;
}

void Sockets::start_listen() {
     _fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(_fd, F_SETFL, O_NONBLOCK);
    if (_fd < 0) {
        throw std::runtime_error("fd socket failed");
    }
    int opt = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        throw std::runtime_error("setsockopt failed");
    }
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(this->_port);
    _addr.sin_addr.s_addr = inet_addr(host.c_str());
    if (bind(_fd, (struct sockaddr*)&_addr, (socklen_t)sizeof(_addr)) == -1) {
        throw std::runtime_error("bind failed");
    }
    if (listen(_fd, 2000) == -1) {
        throw std::runtime_error("listen failed");
    }
}

int Sockets::getFd() const {
    return this->_fd;
}