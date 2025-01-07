#ifndef COMMAND_FACTORY_HPP
# define COMMAND_FACTORY_HPP

# include "CommandCreator.hpp"

class CommandFactory {
	private:
		/* This map stores pointers to CommandCreator objects */
		std::map<std::string, CommandCreator*> commands;

	public:
		CommandFactory(Server& server);
		~CommandFactory();
		CommandPtr createCommand(const std::string& commandName);
};

#endif
