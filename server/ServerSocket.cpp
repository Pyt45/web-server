#include "ServerSocket.hpp"
#include <fcntl.h>

ServerSocket::ServerSocket() {

}

ServerSocket::~ServerSocket() {
    if (_fd != -1) {
        close(_fd);
    }
}

ServerSocket::ServerSocket(ServerSocket const& src) {
			*this = src;
}

ServerSocket& ServerSocket::operator=(ServerSocket const& rhs) {
			if (this != &rhs) {
				_fd = rhs._fd;
				_port = rhs._port;
			}
			return *this;
}

ServerSocket::ServerSocket(int port) {
			_port = port;
}
bool ServerSocket::operator==(ServerSocket const& rhs) {
    if (_port == rhs._port) {
        return true;
    }
    return false;
}

int const& ServerSocket::getPort() const {
    return _port;
}

int const& ServerSocket::getFd() const {
    return _fd;
}

void ServerSocket::socketCreate() {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        std::cerr << "Socket failed" << std::endl;
    } else {
        // fcntl(_fd, F_SETFL, O_NONBLOCK);
    }
}

void ServerSocket::socketBind() {
    if (_fd != -1) {
        // opt
        int opt = 1;
        setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
        
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(_port);
        if (bind(_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Socket bind failed" << std::endl;
            close(_fd);
            _fd = -1;
        }
    }
}

void ServerSocket::socketListen() {
    if (_fd == -1)
        return;
    if (listen(_fd, 5) < 0) {
        std::cerr << "Listen Failed" << std::endl;
        _fd = -1;
    }
}

int ServerSocket::acceptConnection() {
    struct sockaddr_in addr;
    int len = sizeof(addr);
    return accept(_fd, (struct sockaddr*)&addr, (socklen_t*)&len);
}