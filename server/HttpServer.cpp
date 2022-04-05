#include "HttpServer.hpp"

HttpServer::HttpServer() {
    _maxBodySize = -1;
    _stcode = -1;
}

HttpServer::HttpServer(HttpServer const& src) {
    *this = src;
}

std::string HttpServer::_getErrorPages(int st)
{
    return _errors[st];
}

HttpServer& HttpServer::operator=(HttpServer const& rhs) {
    if (this != &rhs) {
        _port = rhs._port;
        _server_name = rhs._server_name;
        _host = rhs._host;
        _root = rhs._root;
        _maxBodySize = rhs._maxBodySize;
        _stcode = rhs._stcode;
        _errors = rhs._errors;
        // methods
        for(size_t i = 0; i < rhs._allowed_methods.size(); i++) {
            _allowed_methods.push_back(rhs._allowed_methods[i]);
        }

         for(size_t i = 0; i < rhs._locations.size(); i++) {
            _locations.push_back(rhs._locations[i]);
        }

    }
    return *this;
}



// ----------------------------GETTERS--------------------------
std::string const& HttpServer::getServerName() const {
    return _server_name;
}

std::vector<std::string> const& HttpServer::getServerNames() const {
    return _server_names;
}

std::string const& HttpServer::getHost() const {
    return _host;
}

std::string const& HttpServer::getRoot() const {
    return _root;
}

std::vector<std::string> const& HttpServer::getAllowedMethods() const {
    return _allowed_methods;
}

std::vector<int> HttpServer::getPort() {
    return _port;
}

int const& HttpServer::getMaxBodySize() const {
    return _maxBodySize;
}

std::vector<Location> HttpServer::getLocations() {
    return _locations;
}

// -----------------------------SETTERS-------------------------
void	HttpServer::setServerName(std::string const& x) {
    _server_name = x;
}

void	HttpServer::setServerNames(std::string const& x) {
    for (size_t i = 0; i < _server_names.size(); i++)
        if (_server_names[i] == x)
            throw std::runtime_error("Syntax Error: 'server_name' duplicated in the same server");
    _server_names.push_back(x);
}

void	HttpServer::setHost(std::string const& x) {
    _host = x;
}

void	HttpServer::setRoot(std::string const& x) {
    _root = x;
}

void	HttpServer::setAllowedMethods(std::vector<std::string> x) {
    for (size_t i = 0; i < x.size(); i++) {
        if (x[i] != "GET" && x[i] != "POST" && x[i] != "DELETE")
            throw std::runtime_error("allowed method not acceptable");
        _allowed_methods.push_back(x[i]);
    }
}

void	HttpServer::setPort(int const& x) {
    for (size_t i = 0; i < _port.size(); i++)
        if (_port[i] == x)
            throw std::runtime_error("Syntax Error: 'Port' duplicated in the same server");
    _port.push_back(x);
}

void	HttpServer::setMaxBodySize(int const& x) {
    _maxBodySize = x;
}

void	HttpServer::addLocation(Location const& loc) {
    _locations.push_back(loc);
}

// ----------------------------OVERLOADS-----------------------

void	HttpServer::addErrorPage(int statusCode, std::string path) {
    _stcode = statusCode;
    _errors.insert(std::pair<int, std::string>(statusCode, path));
    // ErrorPage ep(statusCode, path);
    // _error_pages.push_back(ep);
}

void    HttpServer::start_listen()
{
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
    _addr.sin_port = htons(this->_port[0]);
    _addr.sin_addr.s_addr = inet_addr(_host.c_str());
    if (bind(_fd, (struct sockaddr*)&_addr, (socklen_t)sizeof(_addr)) == -1) {
        throw std::runtime_error("bind failed");
    }
    if (listen(_fd, 2000) == -1) {
        throw std::runtime_error("listen failed");
    }
}

int HttpServer::getFd() const {
    return this->_fd;
}

bool HttpServer::match_server(std::string name) {
    for (size_t i = 0; i < this->_server_names.size(); i++)
    {
        if (name == _server_names[i])
            return true;
    }
    return false;
}