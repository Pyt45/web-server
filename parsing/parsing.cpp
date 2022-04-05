#include "parsing.hpp"
# include <string.h>

bool	pars::_checkbool(std::string str) {
	if (str == "on")
		return (true);
	return (false);
}

bool	pars::_checkCGI(std::string str) {
	if (str != "*.php" && str != "*.js" && str != "*.py")
		return false;
	return true;
}

bool	pars::_setCGI(Location &src) {
	if (src.getUri() == "*.php")
		src.setPhpCGI(true);
	else if (src.getUri() == "*.js")
		src.setNodeCGI(true);
	else if (src.getUri() == "*.py")
		src.setPyCGI(true);
	else
		return false;
	return true;
}

std::vector<std::string> pars::_split(std::string const &str, char sep)
{
    std::vector<std::string> wordsArr;
    std::stringstream ss(str);
    std::string buff;

    while (getline(ss, buff, sep))
        wordsArr.push_back(buff);
    return wordsArr;
}

int		pars::parsLocation(int i, int end, HttpServer& srv) {

	Location	tmp;
	int			open = 0;

	_conf[i].find("{") < _conf[i].length() ? open = 1 : open = 0;

	if (open == 0)
		tmp.setUri(_conf[i].substr(_conf[i].find(":") + 1));
	else if (open == 1)
		tmp.setUri(_conf[i].substr(_conf[i].find(":") + 1, (_conf[i].find("{") - _conf[i].find(":") - 1)));
	if ((_conf[i + 1] != "{" && open == 0) || (_conf[i] == "{" && open == 1))
		throw std::runtime_error("Syntax Error: location `{`");
	open = 1;
	while (open && ++i < end) {
		_conf[i] == "}" ? open = 0 : 1;
		if (_conf[i].compare(0, 8, "location") == 0 || (open == 1 && i + 1 == end))
			throw std::runtime_error("Syntax Error: location `}`");
		if (_conf[i].compare(0, 4, "root") == 0) {
			if (tmp.getRoot() != "")
				throw std::runtime_error("Syntax Error: location: 'root' duplicated");
			tmp.setRoot(_conf[i].substr(_conf[i].find("=") + 1));
		}
		else if (_conf[i].compare(0, 5, "index") == 0) {
			if (tmp.getIndex() != "")
				throw std::runtime_error("Syntax Error: location: 'index' duplicated");
			tmp.setIndex(_conf[i].substr(_conf[i].find("=") + 1));
		}
		else if (_conf[i].compare(0, 15, "allowed_methods") == 0) {
			if (!tmp.getAllowedMethod().empty())
				throw std::runtime_error("Syntax Error: location: 'allowed_methods' duplicated");
			tmp.setAllowedMethods(_split(_conf[i].substr(_conf[i].find("=") + 1), ','));
		}
		else if (_conf[i].compare(0, 9, "autoindex") == 0) {
			if (tmp.getAutoIndex() != false)
				throw std::runtime_error("Syntax Error: location: 'autoindex' duplicated");
			tmp.setAutoIndex(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		}
		else if (_conf[i].compare(0, 9, "redirect=") == 0) {
			if (tmp.getIsRedirect() != false)
				throw std::runtime_error("Syntax Error: location: 'redirect' duplicated");
			tmp.setIsRedirect(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		}
		else if (_conf[i].compare(0, 4, "code") == 0) {
			if (tmp.getStatusCode() != -1)
				throw std::runtime_error("Syntax Error: location: 'status Code' duplicated");
			tmp.setStatusCode(atoi(_conf[i].substr(_conf[i].find("=") + 1).c_str()));
		}
		else if (_conf[i].compare(0, 13, "redirect_path") == 0) {
			if (tmp.getRedirectUrl() != "")
				throw std::runtime_error("Syntax Error: location: 'redirect_path' duplicated");
			tmp.setRedirectUrl(_conf[i].substr(_conf[i].find("=") + 1));
		}
		else if (_conf[i].compare(0, 13, "upload_enable") == 0) {
			if (tmp.getIsUploadEnable())
				throw std::runtime_error("Syntax Error: location: 'upload_enable' duplicated");
			tmp.setIsUploadEnable(_checkbool(_conf[i].substr(_conf[i].find("=") + 1)));
		}
		else if (_conf[i].compare(0, 12, "upload_store") == 0) {
			if (tmp.getUploadDir() != "")
				throw std::runtime_error("Syntax Error: location: 'upload_store' duplicated");
			tmp.setUploadDir(_conf[i].substr(_conf[i].find("=") + 1));
		}
		else if (_conf[i].compare(0, 12, "fastcgi_pass") == 0) {
			if (tmp.getFastcgiPass().length())
				throw std::runtime_error("Syntax Error: location: 'fastcgi_pass' duplicated");
			if (_setCGI(tmp) == false)
				throw std::runtime_error("Syntax Error: location: CGI extension not Allowed");
			tmp.setFastcgiPass(_conf[i].substr(_conf[i].find("=") + 1));
		}
	}

	if (tmp.getIsRedirect() == true && (tmp.getStatusCode() == -1 || tmp.getRedirectUrl() == ""))
		throw std::runtime_error("you need to setup redirect code and index");
	// std::cout << "py_CGI: " << tmp.getPyCGI() << "\tnode_CGI: " << tmp.getNodeCGI() << "\tphp_CGI: " << tmp.getPhpCGI() << std::endl;
	if ((tmp.getUri() == "*.php" || tmp.getUri() == "*.js" || tmp.getUri() == "*.py") && !tmp.getFastcgiPass().length())
		throw std::runtime_error("your CGI extension need to be like this: `fastcgi_pass = on`");

	// if (tmp.getAutoIndex() == true && tmp.getIndex() == "")
	// 	throw std::runtime_error("you need to setup the index, or change auto index to off";

	srv.addLocation(tmp);
	return (i);
}

void	pars::_check_missing(HttpServer &srv) {
	if (srv.getPort().empty())
		throw std::runtime_error("syntax err: Port Not found!");
	else if (srv.getHost() == "")
		throw std::runtime_error("syntax err: Host Not found!");
	else if (srv.getRoot() == "")
		throw std::runtime_error("syntax err: Root Not found!");
}

void	pars::parsServer(int n) {

	int i = 		_servBegin[n];
	HttpServer		_httpServers;

	while (++i < _servEnd[n]) {
		if (_conf[i].compare("server") == 0)
			throw std::runtime_error("Syntax Error: You miss to Close the server `]`");
		if (_conf[i].compare(0, 4, "port") == 0) {
			_httpServers.setPort(atoi(_conf[i].substr(_conf[i].find(":") + 1).c_str()));
		}
		else if (_conf[i].compare(0, 11, "server_name") == 0) {
			_httpServers.setServerName(_conf[i].substr(_conf[i].find(":") + 1));
			_httpServers.setServerNames(_conf[i].substr(_conf[i].find(":") + 1));
		}
		else if (_conf[i].compare(0, 4, "host") == 0) {
			if (_httpServers.getHost() != "")
				throw std::runtime_error("Syntax Error: 'host' duplicated");
			_httpServers.setHost(_conf[i].substr(_conf[i].find(":") + 1));
		}
		else if (_conf[i].compare(0, 4, "root") == 0) {
			if (_httpServers.getRoot() != "")
				throw std::runtime_error("Syntax Error: 'root' duplicated");
			_httpServers.setRoot(_conf[i].substr(_conf[i].find(":") + 1));
		}
		else if (_conf[i].compare(0, 20, "client_max_body_size") == 0) {
			if (_httpServers.getMaxBodySize() != -1)
				throw std::runtime_error("Syntax Error: 'client_max_body_size' duplicated");
			_httpServers.setMaxBodySize(atoi(_conf[i].substr(_conf[i].find(":") + 1).c_str()));
		}
		else if (_conf[i].compare(0, 16, "allowed_methods:") == 0) {
			if (!_httpServers.getAllowedMethods().empty())
				throw std::runtime_error("Syntax Error: 'alowed_method' duplicated");
			_httpServers.setAllowedMethods(_split(_conf[i].substr(_conf[i].find(":") + 1), ','));
		}
		else if (_conf[i].compare(0, 8, "location") == 0) {
			if ((_conf[i].substr(_conf[i].find(":") + 1).compare(0, 1, "/") != 0) && !_checkCGI(_conf[i].substr(_conf[i].find(":") + 1)))
				throw std::runtime_error("Location URI error");
			i = parsLocation(i++, _servEnd[n], _httpServers);
		}
		else if (_conf[i].compare(0, 10, "error_page") == 0) {
			std::string tmp =  _conf[i].substr(_conf[i].find(":") + 1);
			_httpServers.addErrorPage(atoi(tmp.c_str()), tmp.substr(tmp.find(":") + 1));
		}
	}
	_check_missing(_httpServers);
	_Servers.addHttpServer(_httpServers);
}

void	pars::checkServer() {

	int serverClosed = 0;
	for (size_t i = 0; i < _conf.size(); i++) {
		_conf[i].erase(std::remove_if(_conf[i].begin(), _conf[i].end(), ::isspace), _conf[i].end());
		if (serverClosed == 0 && (_conf[i].compare("server") == 0 || _conf[i].compare("server[") == 0)) {
			if (_conf[i + 1].compare("[") != 0 || _conf[i + 1].compare("]") == 0)
				throw std::runtime_error("syntax error: you must open server `[`");
			serverClosed = 1;
			_servBegin.push_back(i + 1);
		}
		else if (serverClosed == 1 &&  _conf[i].compare("]") == 0) {
			_servEnd.push_back(i);
			serverClosed = 0;
			if (_conf[i - 1].compare("[") == 0)
				throw std::runtime_error("syntax error: server is emty");
			if ((i + 1) != _conf.size() && _conf[i + 1].compare("server") != 0 && _conf[i + 1].compare(0, 1, "#") != 0)
				throw std::runtime_error("syntax error: at end of server");
		}
	}
	if (_servBegin.size() && (_servBegin.size() != _servEnd.size()))
		throw std::runtime_error("syntax error: server is emty");
	for (size_t i = 0; i < _servBegin.size(); i++)
		parsServer(i);
}

pars::pars(std::string fileName): _Servers(Server::getInstance()) {

	std::string buff;
	std::ifstream readFile(fileName);

	if (access( fileName.c_str(), F_OK ))
		throw std::runtime_error("file doesn't exist!");
	if (fileName.compare(fileName.length() - 5, fileName.length(), ".conf") != 0)
		throw std::runtime_error("config file must be '.conf'");
	while (getline(readFile, buff)) {
		std::string tmp = buff.substr(0, buff.size());
		if (tmp != "")
			_conf.push_back(tmp);
	}
	readFile.close();
	if (_conf.empty())
		throw std::runtime_error("can't read the file!");
	checkServer();
}

pars::~pars() {
}
