#ifndef COMMAND_PARSER_HPP
# define COMMAND_PARSER_HPP

# include "Client.hpp"
# include "Server.hpp"

class Client;
class CommandFactory;
class Server;
class Channel;

class CommandParser {
	private:
		CommandFactory* commandFactory;
		Server& _server;
	public:
		CommandParser(Server& server);
		~CommandParser();
		void parseAndExecute(Client& client, const std::string& message, std::map<std::string, Channel*>& channel);
};

#endif
