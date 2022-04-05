#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <vector>
# include <algorithm>
# include <iostream>
# include <sstream>
# include <map>
# include <string>
# include "../server/HttpServer.hpp"
# include "Regex.hpp"

class Request {
    public:
        struct ArgContent {
			std::string _Cdisp;
			std::string _Ctype;
			std::string _data;
        };
        Request();
        ~Request();
        int _fd;
        void _parseIncomingRequest(const std::string& _buffer);
        void _parseLine(const std::string& _line);
        unsigned int _getPostLenght(std::string data, std::string boundary);
        std::string _getHeaderContent(std::string _first);
        void _pushDataToArg(std::string _data);
        void _pushToPostBody(std::string _data);
        ArgContent _pushToArg(std::string _data);
        ArgContent _getArg(size_t i);
        std::vector<ArgContent> _getVecCont() const;
        bool _matchBegin(std::string& _regex, std::string& _line);
        int _getContentLen();
        int _getError();
        std::string _getCgiUriFile();
        void _parseQueryString(std::string uri);
        std::string _getPostBody() const;
        std::string _getPostCgi() const;
        std::string _getBoundary() const;
        void    _setIterator(std::vector<HttpServer>::iterator it);
        void _setStatus(int st);
        std::string		_getVal(std::string data, std::string _regex, bool boundary, bool self);
        // Check Error
        void _checkError();
    private:
        int             _st;
		bool            _isDone;
		unsigned int    _Clen;
		std::vector<std::string> _parse;
		std::map<std::string, std::string> _rmap;
		std::vector<ArgContent> _aCont;
		bool            _isArg;
		std::string     _argBody;
		std::string     _boundary;
		std::string     _body;
		std::vector<HttpServer>::iterator _it;
		int _error;
        std::string     _postBody;
        std::string     _postCgi;
		bool            _isLine;
		bool            _isL;
        std::string     _host;
};

#endif