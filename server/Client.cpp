#include "Client.hpp"
#include <cstdlib>
#include "Server.hpp"
#include "UnchunkContent.hpp"
#include "RequestValidator.hpp" 


Client::Client(int server_fd) {
	status = 1;
	close = false;
	is_chunked = false;
    int len = sizeof(addr);
    _ready = false;
    _conn = accept(server_fd, (struct sockaddr*)&addr, (socklen_t*)&len);
    if (_conn < 0) {
        throw std::runtime_error("accept failed");
    }
    fcntl(_conn, F_SETFL, O_NONBLOCK);
    pfd.events = POLLIN;
    pfd.fd = _conn;
	_req = "";
}

bool	Client::getReady()
{
    return this->_ready;
}

bool	Client::_matchBegin(std::string _regex, std::string _line)
{
	std::string _r = _regex;
	_r.pop_back();

	return _line.compare(0, _r.size(), _r) == 0;
}


void	Client::_handleResponse(Request req, std::vector<HttpServer>::iterator it)
{
    std::vector<Location> Locations = it->getLocations();
	std::vector<Location>::iterator lit = Locations.begin();
	Location tmp;
	std::string _match;
	std::string *path = new std::string("/");
	std::string uri = req._getHeaderContent("uri");

	for (; lit != Locations.end(); lit++)
	{
		if (lit->getFastcgiPass().length())
		{
			if (uri.find(".") != std::string::npos)
				_match = uri.substr(uri.find("."), uri.length());
			if (_match.find("?") != std::string::npos)
				_match = _match.substr(0, _match.find("?"));
			if ((lit->getPhpCGI() && !_match.compare(".php"))
			|| (lit->getPyCGI() && !_match.compare(".py"))
			|| (lit->getNodeCGI() && !_match.compare(".js")))
			{
				tmp = *lit;
				break ;
			}
		}
		else if (_matchBegin(lit->getUri(), uri))
		{
			if (lit->getUri().compare(uri) == 0)
			{
				tmp = *lit;
				break ;
			}
			else if (path->compare("/") == 0)
			{
				*path = lit->getUri();
				tmp = *lit;
			}
			else if (path->length() <= lit->getUri().length())
			{
				*path = lit->getUri();
				tmp = *lit;
			}
		}
	}
	delete path;
	Response res = Response(tmp, *it);


	res._setRequest(req);
	res._startResponse();
	responseContent = res._getResContent();
	close = res._toClose();

}

void	Client::_readHeader(std::string con)
{
	size_t pos = con.find("\r\n\r\n");
	std::string headers = con.substr(0, pos);
	if ((pos = headers.find("Host:")) != std::string::npos)
	{
		size_t newline = headers.substr(pos).find("\r\n");
		std::string tmp = headers.substr(pos).substr(0, newline);
		_req = tmp.substr(6);
	}
}

void Client::_handleRequest(std::vector<HttpServer>::iterator it)
{
	Request req;

	req._setIterator(it);
	req._setStatus(status);
	req._parseIncomingRequest(content);
	_handleResponse(req, it);
}

void	Client::setReady(bool x) {
    if (x)
	{
		sended = 0;
		if (is_chunked)
		{
			try
			{
				std::string tmp = UnchunkContent::loop_over_numbers(content.substr(content.find("\r\n\r\n") + 4));
				size_t k = content.find("\r\n\r\n");
				content = content.substr(0, k);
				if (content.find("GET") == std::string::npos && content.find("DELETE") == std::string::npos)
					content += "\r\nContent-Length: " + std::to_string(tmp.size());
				content += "\r\n\r\n";
				content += tmp;
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
		// handle request;
		std::vector<HttpServer>::iterator it = Server::getInstance().getHttpServers().begin();
		bool found = false;
		_readHeader(content);
		size_t pos = _req.find(":");
        if (pos != std::string::npos)
            _req = _req.substr(0, pos);
		while (it != Server::getInstance().getHttpServers().end() && status == 1)
		{
			if (it->match_server(_req))
			{
				found = true;
				break;
			}
			it++;
		}
		if (found)
			_handleRequest(it);
		else
		{
			it = Server::getInstance().getHttpServers().begin();
			_handleRequest(it);
		}
		sended = 0;
		is_chunked = false;
		pfd.events = POLLOUT;
	}
	else
	{
		this->content = "";
		pfd.events = POLLIN;
	}
	this->_ready = x;
}

int Client::getConnection() {
    return this->_conn;
}

struct pollfd Client::getPfd() {
        return this->pfd;
}

int checkEnd(const std::string& str, const std::string& end)
{
        size_t  i = str.size();
        size_t  j = end.size();

        while (j > 0)
        {
                i--;
                j--;
                if (i < 0 || str[i] != end[j])
                        return (1);
        }
        return (0);
}

std::string Client::getResponseContent() {
        return this->responseContent;
}

std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

void Client::writeConnection() {
	int r = send(_conn, responseContent.c_str(), responseContent.size(), 0);
	if (r == -1 || r == 0) {
		throw std::runtime_error("should close the connection");
	}
	setReady(false);
}

Client::~Client() {
}

bool Client::is_numeric(std::string str) {
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
}

int Client::readConnection() {
    char buffer[16000];
    int r = recv(_conn, buffer, 15999, 0);
    if (r == -1)
        return 1;
    if (r == 0)
        throw std::runtime_error("Closed");
    else {
		std::string tmp;
		buffer[r] = '\0';
		tmp.assign(buffer);
		content += tmp;
		size_t j = content.find("\r\n\r\n", 0);
		if (j != std::string::npos)
		{
			std::string headers = content.substr(0, j + 2);
			try
			{
				size_t newline = headers.find("\n");
				if (!RequestValidator::validRequest(headers.substr(0, newline).append("\n")))
					;
			}
			catch(const std::exception& e)
			{
				status = 505;
				return 0;
			}
			;
			if (!RequestValidator::validHeaders(headers))
			{
				status = 400;
				return (0);
			}
			if (content.find("Content-Length: ") == std::string::npos) {
				if (content.find("Transfer-Encoding: chunked") != std::string::npos) {
					if (checkEnd(content, "0\r\n\r\n") == 0)
					{
						is_chunked = true;
						return (0);
					}
					else
						return (1);
				}
				else
					return (0);
			}
			std::string _tmp = content.substr(content.find("Content-Length: "));
			size_t pos = _tmp.find("\r\n");
			if (!is_numeric(_tmp.substr(16, pos - 16))) {
				status = 400;
				return 0;
			}
			int len = std::atoi(content.substr(content.find("Content-Length: ") + 16, 10).c_str());
			if (len < 0)
			{
				status = 400;
				return (0);
			}
			std::string tmp = content.substr(j + 4);
			if (int(tmp.size()) >= len)
				return (0);
			else
				return (1);
		}
    }
    return (1);
}

std::string Client::getContent() const {
    return this->content;
}