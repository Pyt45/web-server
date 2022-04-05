
#include "./server/Server.hpp"
#include "./parsing/parsing.hpp"

int		main(int argc, char **argv)
{
	try {
		if (argc > 1)
		{
			std::string filename(argv[1]);
			pars ser(filename);
		}
		else
		{
			pars ser("./config/webServ.conf");
		}
		Server& srv = Server::getInstance();
		srv.acceptConnections();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
	return (0);	
}
