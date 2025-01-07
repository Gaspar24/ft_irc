#include "Client.hpp"
#include <unistd.h>
#include <cstring>
# include "NumericMessages.hpp"

Client::Client(int fd) : _clientFD(fd), _authenticated(false), _serverOperator(false), _welcomeMessage(false), nickname(""), username(""), _buffer(""), color(getRandomColorFmt(1, 1))
{
	pthread_mutex_init(&clientMutex, NULL);
}

void Client::sendMessage(const std::string &message)
{
	pthread_mutex_lock(&clientMutex);
	std::string msg = message;
	if (msg.find("\r\n") == std::string::npos) {
		msg += "\r\n";
	}
	send(_clientFD, msg.c_str(), msg.length(), 0);
	pthread_mutex_unlock(&clientMutex);
}

Client::~Client() {
	pthread_mutex_destroy(&clientMutex);
}

void Client::handleRead() {
	char buffer[MAX_BUFFER];
	ssize_t nbytes = recv(_clientFD, buffer, sizeof(buffer) - 1, 0);
	if (nbytes < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // No data available
		else
			throw std::runtime_error(std::string(__func__) + ": " + std::string(strerror(errno)));
	}
	else if (nbytes == 0)
	{
		throw std::runtime_error("Client disconnected: " + toStr(_clientFD));
	}
	buffer[nbytes] = '\0';
	this->_buffer += buffer;
	std::ostringstream oss;
	oss << color << "Received message: " << buffer << C_END;
	Utils::safePrint(oss.str());

	// Process commands
	Server* server = Server::getInstance();
	CommandParser commandParser(*server);
	size_t pos;
	while ((pos = this->_buffer.find_first_of("\r\n")) != std::string::npos)
	{
		std::string command = this->_buffer.substr(0, pos);
		this->_buffer.erase(0, pos + 2);

		Utils::safePrint(color + "Processing command: " + command + std::string(C_END));
		commandParser.parseAndExecute(*this, command, server->getChannels());
	}
}

bool Client::isAuthenticated() const {
	return _authenticated;
}

void Client::setAuthenticated(bool flag) {
	_authenticated = flag;
}

void Client::setServerOperator(bool flag) {
	_serverOperator = flag;
}

int Client::getFd() const
{
	return _clientFD;
}

bool Client::getServerOperator() const {
	return _serverOperator;
}

bool Client::hasReceiveWelcomeMessage() const {
	return _welcomeMessage;
}

void Client::setReceivedWelcomeMessage(bool flag) {
	_welcomeMessage = flag;
}

void Client::checkAndSendWelcomeMessage() {
	if (isAuthenticated() && !nickname.empty() && !username.empty() && !realname.empty()) {
		if (!hasReceiveWelcomeMessage()) {
			std::string response = RPL_WELCOME(nickname);
			sendMessage(response);
			setReceivedWelcomeMessage(true);
		}
	}
}

void Client::setRegistered(bool flag) {
	_registered = flag;
}

bool Client::isRegistered() const {
	return _registered;
}