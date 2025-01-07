# include "CommandParser.hpp"
# include "CommandFactory.hpp"
# include "NumericMessages.hpp"
# include "Utils.hpp"

CommandParser::CommandParser(Server& server) : _server(server) {
	commandFactory = new CommandFactory(server);
}

CommandParser::~CommandParser() {
	delete commandFactory;
}

void CommandParser::parseAndExecute(Client& client, const std::string& message, std::map<std::string, Channel*>& channels) {
	std::istringstream stream(message);
	std::string commandName;
	std::string args;

	stream >> commandName;
	std::getline(stream, args);
	// args = Utils::trim(args);
	
	// std::cout << "cmdName: " << commandName << std::endl;
	// std::cout << "Args: " << args << std::endl;
	if (commandName[0] == '#') {
		Client *clientPtr = &client;
		Channel* targetChannel = _server.getChannel(commandName);
		if (!targetChannel) {
			client.sendMessage(ERR_NOSUCHCHANNEL(commandName));
			return;
		}
		if (targetChannel->isMember(clientPtr)) {
			targetChannel->broadcast(args, clientPtr);
			Utils::safePrint("printing after broadcast in parseAndExecute");
		} else {
			client.sendMessage(ERR_CANNOTSENDTOCHAN(commandName));
		}
		return;
	}
		CommandPtr command = commandFactory->createCommand(commandName);
		if (command) {
			try	{
				command->execute(client, args, channels);
			}
			catch(const std::exception& e)	{
				delete command;
				throw;
			}
			delete command;

			// Check if client has completed the registration
			client.checkAndSendWelcomeMessage();
		} else {
			std::string response = ERR_UNKNOWNCOMMAND(commandName);
			client.sendMessage(response);
		}
}
