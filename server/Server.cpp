#include "Server.hpp"
#include "poll.h"
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

Server& Server::getInstance() {
    if (!_instance) {
        _instance = new Server();
    }
    return *_instance;
}

Server::~Server() {
    delete _instance;
}

void Server::addHttpServer(HttpServer const& server) {
    _http_servers.push_back(server);
}

std::vector<ServerSocket> const& Server::getSockets() const {
    return _sockets;
}

std::vector<HttpServer>& Server::getHttpServers() {
    return _http_servers;
}

// polymorph socketCreate
void Server::socketsCreate() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        it->socketCreate();
    }
}

void Server::socketsBind() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        it->socketBind();
    }
}

void Server::socketsListen() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        it->socketListen();
    }
}

Server::Server() {

}

void Server::addSocket(ServerSocket const& ss) {
    std::vector<ServerSocket>::iterator it;
    it = std::find(_sockets.begin(), _sockets.end(), ss);
    if (it == _sockets.end()) {
        _sockets.push_back(ss);
    }
}

Server* Server::_instance = nullptr;

void Server::addServerSocketsToSet() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        if (it->getFd() != -1) {
            FD_SET(it->getFd(), &master_set);
        }
    }
}

std::vector<ServerSocket>::iterator Server::isServerSocket(int fd) {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        if (it->getFd() == fd) {
            return it;
        }
    }
    return it;
}

void    Server::init_poll(std::vector<struct pollfd>& fds) {
    std::vector<HttpServer>::iterator h_it;

    // server fds
    h_it = _http_servers.begin();
    while (h_it != _http_servers.end()) {
        fds.push_back((struct pollfd){h_it->getFd(), POLLIN, 0});
        h_it++;
    }
    // client fds
    std::vector<Client>::iterator c_it;
    c_it = _clients.begin();
    while (c_it != _clients.end())
    {
        fds.push_back(c_it->getPfd());
        c_it++;
    }
}

void Server::acceptIncomingConnection(std::vector<struct pollfd>& fds, std::vector<Client>& clients) {
    for(size_t i = 0; i < _http_servers.size(); i++) {
        if (fds[i].revents & POLLIN) {
            try
            {
                Client c(fds[i].fd);
                clients.push_back(c);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
}

void Server::poll_handle(std::vector<struct pollfd>& fds) {
    int n = poll(&(*fds.begin()), fds.size(), 3000);
    std::vector<Client> new_clients;
    if (n == 0) {
        std::cout << "Timeout" << std::endl;
    } else if (n > 0) {
        for (size_t i = 0; i < fds.size(); i++) {
            if (i < _http_servers.size()) {
                if (fds[i].revents & POLLIN) {
                    try
                    {
                        Client c(fds[i].fd);
                        new_clients.push_back(c);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
            }
        }
        _clients.assign(new_clients.begin(), new_clients.end());
    } else if (n == -1) {
        exit(EXIT_FAILURE);
    }
}

void Server::make_sockets() {
    std::set<int> _ports_set;
    std::vector<HttpServer>::iterator it;
    std::vector<Sockets> _sockets;

    it = _http_servers.begin();
    while (it != _http_servers.end())
    {
        std::vector<int> _ports = it->getPort();
        for (size_t i = 0; i < _ports.size(); i++)
        {
            std::set<int>::iterator s_it = _ports_set.find(_ports[i]);
            if (s_it == _ports_set.end())
            {
                Sockets s(_ports[i], it->getHost());
                _ports_set.insert(_ports[i]);
                this->_socks.push_back(s);
            }
        }
        it++;
    }
}

void Server::start_servers() {
    make_sockets();
    std::vector<Sockets>::iterator s_it;

    s_it = _socks.begin();
    while (s_it != _socks.end())
    {
        try
        {
            s_it->start_listen();
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            s_it = _socks.erase(s_it);
            continue;
        }
        s_it++;        
    }
}

void Server::acceptConnections() {
    std::cout << "start servers" << std::endl;
    start_servers();
    if (_socks.size() == 0) {
        throw std::runtime_error("no server running");
    }
    std::vector<Sockets>::iterator s_it;
    while (true)
    {
        std::vector<struct pollfd> fds;
        // server fds
        s_it = _socks.begin();
        while (s_it != _socks.end()) {
            fds.push_back((struct pollfd){s_it->getFd(), POLLIN, 0});
            s_it++;
        }
        // client fds
        std::vector<Client>::iterator c_it;
        c_it = _clients.begin();
        while (c_it != _clients.end())
        {
            fds.push_back(c_it->getPfd());
            c_it++;
        }

    int n = poll(&(*fds.begin()), fds.size(), 10000);
    std::vector<Client> new_clients;
    std::set<int> toRemove;
	if (n == 0) {
            std::cout << "Timeout" << std::endl;
    } else if (n > 0) {
        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (i < _socks.size()) {
                    try
                    {
                        Client c(fds[i].fd);
                        new_clients.push_back(c);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                } else {
                    // read connection
                    try
                    {
                        if (!_clients[i - _socks.size()].readConnection()) {
                            _clients[i - _socks.size()].setReady(true);
                        }
                    }
                    catch(const std::exception& e)
                    {
                            toRemove.insert(i - _socks.size());
                    }
                }
            } else if (fds[i].revents & POLLOUT) {
                try
                {
                    _clients[i - _socks.size()].writeConnection();
                    if (_clients[i - _socks.size()].close)
                        toRemove.insert(i - _socks.size());
                }
                catch(const std::exception& e)
                {
                    toRemove.insert(i - _socks.size());
                }
            } else if (fds[i].revents & POLLHUP) {
                toRemove.insert(i - _socks.size());
            }
        }
    }
	std::set<int>::iterator it_set = toRemove.begin();
    it_set = toRemove.begin();
    std::vector<Client> tmp;
	for (size_t i = 0; i < _clients.size(); i++)
    {
        if (toRemove.find(i) == toRemove.end()) {
            tmp.push_back(_clients[i]);
        } else {
            close(_clients[i].getConnection());
        }
    }
    _clients.assign(tmp.begin(), tmp.end());
    _clients.insert(_clients.end(), new_clients.begin(), new_clients.end());
    }
}
