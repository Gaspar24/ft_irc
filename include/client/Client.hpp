#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <pthread.h>
# include <string>
# include <Utils.hpp>
# include <cerrno>
# include <cstring> // strerror
# include <sys/socket.h>
#include <vector>
#include <Channel.hpp>

# include "CommandParser.hpp"

# ifndef DEBUG
#  define DEBUG 0
# endif


# ifndef MAX_BUFFER
# define MAX_BUFFER 4096
# endif

class Channel;

class Client 
{
	private:
		int _clientFD;
		bool _authenticated;
		bool _registered;
		bool _serverOperator;
		bool _welcomeMessage;
	public:
		std::string nickname;
		std::string username;
		std::string realname;
		std::string _buffer;
		std::string color;
		pthread_t thread;
		pthread_mutex_t clientMutex;

		Client(int fd);
		~Client();
		int getFd() const;
		void sendMessage(const std::string &message);
		void handleRead();
		bool isAuthenticated() const;
		bool hasReceiveWelcomeMessage() const;
		void checkAndSendWelcomeMessage();

		// Setter
		void setAuthenticated(bool);
		void setServerOperator(bool);
		void setReceivedWelcomeMessage(bool);
		void setRegistered(bool);

		// Getters
		bool getServerOperator() const;
		std::string getClientNick() const {return this->nickname;}
		std::string getNick() const {return nickname;}
		int getSocket() const { return _clientFD; }
		bool isRegistered() const;

};

// std::ostream& operator << (std::ostream& os, Client& rhs);

#endif // CLIENT_HPP