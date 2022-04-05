#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <vector>
# include "HttpServer.hpp"
# include "ServerSocket.hpp"
# include <algorithm>
# include <set>
# include "Client.hpp"
#include "Sockets.hpp"


class Server {
	public:
		~Server();
		static Server& getInstance();

		void addHttpServer(HttpServer const& server);

		std::vector<ServerSocket> const& getSockets() const;

		std::vector<HttpServer>& getHttpServers();

		void double_servers();
		// polymorph socketCreate
		void socketsCreate();

		void socketsBind();
		void socketsListen();

		void	acceptConnections();

		void make_sockets();
		void addServerSocketsToSet();
		void start_servers();
		void poll_handle(std::vector<struct pollfd>& fds);
		void acceptIncomingConnection(std::vector<struct pollfd>& fds, std::vector<Client>& clients);

		std::vector<ServerSocket>::iterator isServerSocket(int fd);
	private:
		Server();
		static Server* _instance;
		std::vector<HttpServer> _http_servers;
		std::vector<ServerSocket> _sockets;
		std::vector<Client> _clients;
		std::vector<Sockets> _socks;

		fd_set master_set;

		// add socket
		void addSocket(ServerSocket const& ss);
		// initialize the poll fds for each loop
		void    init_poll(std::vector<struct pollfd>& fds);
};
#endif
