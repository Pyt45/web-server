#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <netinet/in.h>
# include <sys/socket.h>
# include <iostream>
# include <fcntl.h>
# include <poll.h>
# include <unistd.h>
# include "HttpServer.hpp"
# include "../includes/Request.hpp"
# include "../includes/Response.hpp"
// class Request;
// class Response;
class Client {
    public:
        bool close;
        int status;

        Client(int server_fd);

        int getConnection();
        int readConnection();
	void writeConnection();
        std::string getContent() const;
        bool getReady();
        void setReady(bool x);

        struct pollfd getPfd();
        std::string getResponseContent();
        void _handleRequest(std::vector<HttpServer>::iterator it);
        void _handleResponse(Request req, std::vector<HttpServer>::iterator it);
        bool	_matchBegin(std::string _regex, std::string _line);
		void _readHeader(std::string con);
        bool    is_numeric(std::string str);
	~Client();
        bool is_chunked;
    private:
	int sended;
        int _conn;
        struct sockaddr_in addr;
        bool _ready;
		std::string _req;
	std::string responseContent;
        std::string content;
        struct pollfd pfd;
};
#endif
