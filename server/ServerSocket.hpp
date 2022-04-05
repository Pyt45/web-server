#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iostream>

class ServerSocket {
	public:
		ServerSocket();
		~ServerSocket();
		ServerSocket(ServerSocket const& src);
		ServerSocket& operator=(ServerSocket const& rhs);
		ServerSocket(int port);
		bool operator==(ServerSocket const& rhs);
		
		int const& getPort() const;
		
		int const& getFd() const;

		void socketCreate();

		void socketBind();

		void socketListen();

		int acceptConnection();

	private:
		int _fd;
		int _port;
};
// std::ostream& operator<<(std::ostream& o, ServerSocket const& rhs) {
// 	o << "Socket:";
// 	o << std::endl << "Fd :" << rhs.getFd();
// 	o << std::endl << "Port: " << rhs.getPort();
// 	o << std::endl;
// 	return o;
// }
#endif
