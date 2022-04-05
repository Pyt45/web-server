#include "Location.hpp"

Location::Location() {
    _uri = "/";
    _status_code = -1;
    _auto_index = false;
    _is_redirect = false;
    _isUploadEnable = false;
    
    // CGI
    _fastcgi_pass = "";
    _php_CGI = false;
    _node_CGI = false;
    _py_CGI = false;
}

Location::Location(Location const & rhs)
{
    this->operator=(rhs);
    *this = rhs;

}

Location::~Location() {
    
}

Location& Location::operator=(Location const& rhs) {
    if (this != &rhs) {
        _uri = rhs._uri;
        _root = rhs._root;
        _index = rhs._index;
        _auto_index = rhs._auto_index;

        // Upload
        _uploadDir = rhs._uploadDir;
        _isUploadEnable = rhs._isUploadEnable;

        // Redirect
        _is_redirect = rhs._is_redirect;
        _redirect_url = rhs._redirect_url;
        _status_code = rhs._status_code;

        // CGI
        _fastcgi_pass = rhs._fastcgi_pass;
        _php_CGI = rhs._php_CGI;
        _node_CGI = rhs._node_CGI;
        _py_CGI = rhs._py_CGI;

        // methods
        for(size_t i = 0; i < rhs._allowed_methods.size(); i++) {
            _allowed_methods.push_back(rhs._allowed_methods[i]);
        }
    }
    return *this;
}

void        Location::setIsUploadEnable(bool x) { _isUploadEnable = x; }
void        Location::setUploadDir(std::string dir) { _uploadDir = dir; }
void		Location::setUri(std::string const& x) {	_uri = x; }
void		Location::setRoot(std::string const& x) {	_root = x; }
void		Location::setIndex(std::string const& x) {	_index = x; }
void		Location::setAllowedMethods(std::vector<std::string> x) {
    for (size_t i = 0; i < x.size(); i++) {
        if (x[i] != "GET" && x[i] != "POST" && x[i] != "DELETE")
            throw "allowed method not acceptable";
        _allowed_methods.push_back(x[i]);
    }
}
void		Location::setAutoIndex(bool x) {	_auto_index = x; }
void		Location::setFastcgiPass(std::string x) {	_fastcgi_pass = x; }
void		Location::setPhpCGI(bool x) {	_php_CGI = x; }
void		Location::setNodeCGI(bool x) {	_node_CGI = x; }
void		Location::setPyCGI(bool x) {	_py_CGI = x; }
void		Location::setIsRedirect(bool x) {	_is_redirect = x; }
void		Location::setStatusCode(int x) {	_status_code = x; }
void		Location::setRedirectUrl(std::string const& x) {	_redirect_url = x; }

bool		Location::getAutoIndex() const { return (_auto_index); }
std::string	Location::getFastcgiPass() const { return (_fastcgi_pass); }
bool		Location::getPhpCGI() const { return (_php_CGI); }
bool		Location::getNodeCGI() const { return (_node_CGI); }
bool		Location::getPyCGI() const { return (_py_CGI); }
bool		Location::getIsRedirect() const { return (_is_redirect); }
bool		Location::getIsUploadEnable() const { return (_isUploadEnable); }
std::string		Location::getUploadDir() const { return (_uploadDir); }
int			Location::getStatusCode() const { return (_status_code); }
std::string	Location::getUri() const { return (_uri); }
std::string	Location::getRoot() const { return (_root); }
std::string	Location::getIndex() const { return (_index); }
std::string	Location::getRedirectUrl() const { return (_redirect_url); }
std::vector<std::string>	Location::getAllowedMethod() const { return (_allowed_methods); }


void		Location::checkVal() {
    std::cout << "URI: \t\t" << _uri << "\t\tRoot: \t\t" << _root << std::endl;
    std::cout << "index: \t\t" << _index << "\tAllowed Method: ";
    for (size_t i = 0; i < _allowed_methods.size(); i++)
        std::cout << _allowed_methods[i] << " ";
    std::cout << std::endl;
    std::cout << "auto_index: \t" << std::boolalpha << _auto_index << "\t\tredirect: \t" << _is_redirect << std::endl;
    std::cout << "code: \t\t" << _status_code << "\t\tredirect_path: \t" << _redirect_url << std::endl;
    std::cout << "============================" << std::endl;
}
