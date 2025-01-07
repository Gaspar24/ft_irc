#include <iostream>
#include "Client.hpp"
#include "Server.hpp"
#include <string>

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
			throw std::invalid_argument(error("Invalid arguments.", 0));

		int port;
		std::stringstream ss(argv[1]);
		if (!(ss >> port) || !(ss.eof()))
			throw std::invalid_argument(error("Invalid port number", 0));
		if ((port < 6660 || port > 6669) && (port < 7000 || port > 7005))
			throw std::out_of_range(error("Port number out of range: ", 0) + toStr(port));
		std::string password(argv[2]);
		
		Server server(port, password);
		server.run();
		return 0;
	}
	catch (std::invalid_argument const& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (std::out_of_range const& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::string str(argv[0]);
	std::cerr << "Usage:" << std::endl ;
	std::cerr << getColorStr(FLGREEN, str + " <IRC_port> <pass>") << std::endl ;
	return 1;
}