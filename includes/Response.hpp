
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include <stdexcept>
# include <unistd.h>
# include <fstream>
# include <map>
# include <sys/stat.h>
# include <cstdio>
# include <dirent.h>
# include <sys/types.h>
# include "../server/Location.hpp"
# include "Regex.hpp"

# define S_OK 200
# define S_MOVED_PERM 301
# define S_TEMP_REDIR 307
# define S_BAD_REQ 400
# define S_FORBIDDEN 403
# define S_NOT_FOUND 404
# define S_METHOD_NOT_ALLOWED 405
# define S_LENGTH_REQUIRED 411
# define S_PAY_LOAD_TOO_LARGE 413
# define S_URI_TOO_LONG 414
# define S_UNSUPPORTED_MEDIA_TYPE 415
# define S_INTERNAL_SERVER_ERROR 500
# define S_NOT_IMPLEMENTED 501
# define S_BAD_GATEWAY 502
# define S_GATEWAY_TIMEOUT 504
# define S_HTTP_VERSION_NOT_SUPPORTED 505
// AND SOME OTHER STATUS

// DEBUG MODE
# define DEBUG_MODE 0

class Response {
	private:
		Request _request;
		Location& _location;
		HttpServer& _httpServ;
		size_t  _status;
		std::string _body;
		std::string _ResponseContent;
		std::map<int, std::string> _stResp;
		std::string _scriptFileName;
		std::string _Ctype;
		std::string _st;
		std::string _loc;
		std::string _headers;
	public:
		Response(Location& location, HttpServer& httpServ);
		Response(Response const & rhs);
		Response& operator=(Response const & rhs);
		~Response();


		void _handleCGI();
		int _isCGI();
		bool _matchBegin(std::string _regex, std::string _line);
		void _setRequest(Request& req);
		void _setLocation(Location& location);
		Location _getLocation() const;
		void _startResponse();
		void _applyMethod();
		void _handleRedirect();
		int _checkAllowedMethod(std::string method);
		std::string _getResContent() const;
		void _makeStatus();
		int _checkPermission(std::string path, int mode);
		std::string _getFilePath(std::string uri);
		std::string _getErrorPagePath(std::string filename);
		std::string _getFileNameFromUri(std::string uri);
		std::string _getDir(void);
		std::string _getUploadDir();
		std::string _generateHtmlTemplate();
		std::string _getHrefLink(std::string dirname);
		void		_applyAutoIndexing(std::string _dir);
		int			_isDir(std::string dirname);
		void _applyGetMethod();
		void _readFile(std::string file);
		void _readErrorPageFile(std::string file);
		void _applyPostMethod();
		std::string _getFileNameFromDisp(std::string disp);
		void _applyDeleteMethod();
		void _generateErrorPage();
		void _deleteFile(std::string _file);
		std::string _getScriptFileName() const;
		int _runCgi();
		std::string _getContentType();
		bool        _matchEnd(std::string s1, std::string s2);
		void        _handleError();
		bool 		_toClose();
		void 		_setCookie();
		std::vector<std::string>	_split(std::string s, std::string delimiter);
		void 	_handleRequestError(int st);
		void	_RenderResponse();
		std::string		_getVal(std::string data, std::string _regex, bool boundary, bool self);
};

#endif
