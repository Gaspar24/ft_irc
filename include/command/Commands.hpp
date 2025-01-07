#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include "ICommand.hpp"
# include "Server.hpp"
# include "Channel.hpp"

class Client;

enum CommandType {
	CAP,
	PASS,
	NICK,
	USER,
	QUIT,
	PING,
	PONG,
	OPER,
	PRIVMSG,
	JOIN,
	TOPIC,
	PART,
	KICK,
	MODE,
	INVITE,
	WHO
};

class Command : public ICommand {
	private:
		CommandType _type;
		Server& _server;

		// Command Methods
		void handleCap(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handlePass(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleNick(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleUser(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleQuit(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handlePing(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handlePong(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleOper(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handlePrivMsg(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleWho(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		//Channel
		void handleJoin(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleTopic(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handlePart(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleKick(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleMode(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		void handleInvite(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);

		// ReturnType (ClassName::*PointerName)(ParameterTypes)
		typedef void(Command::*CommandHandler) (Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
		std::map<CommandType, CommandHandler> commands;

	public:
		Command(CommandType type, Server& server);
		~Command();
		void execute(Client& client, const std::string& args, std::map<std::string, Channel*>& channels);
};

#endif
