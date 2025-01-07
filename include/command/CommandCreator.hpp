#ifndef COMMAND_CREATOR_HPP
#define COMMAND_CREATOR_HPP

# include "ICommand.hpp"
# include "Commands.hpp"

typedef ICommand* CommandPtr;

class CommandCreator {
	public:
		virtual ~CommandCreator() {}
		virtual CommandPtr operator()() const = 0;
};


/* This template class implements the operator() to create instances
	of specific command types */
template <typename T>
class CommandCreatorImpl : public CommandCreator {
	public:
		CommandCreatorImpl(CommandType type, Server& server);
		CommandPtr operator()() const;
	private:
		CommandType _type;
		Server& _server;
};

#endif
