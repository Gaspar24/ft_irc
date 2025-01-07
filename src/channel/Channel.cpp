#include "Channel.hpp"
#include <algorithm>

Channel::~Channel()
{
}

void Channel::addMember(Client *client)
{
	members.push_back(client);
}
void Channel::addOperator(Client *client) {
	operators.push_back(client);
}

void Channel::addAllowedPeople(Client *client) {
	people.push_back(client);
}

void Channel::removeMember(Client *client) {
	
	std::vector<Client *>::iterator it = std::remove(members.begin(), members.end(), client);
	if (it != members.end()) {
		members.erase(it, members.end());
	}
}


void Channel::removeOperator(Client *client) {
	std::vector<Client *>::iterator it = std::remove(operators.begin(), operators.end(), client);
	if (it != operators.end()) {
		operators.erase(it, operators.end());
	}
}

void Channel::removePeople(Client *client) {
	std::vector<Client *>::iterator it = std::remove(people.begin(), people.end(), client);
	if (it != people.end()) {
		people.erase(it, people.end());
	}
}

void Channel::broadcast(const std::string &message, Client *sender) {
	std::string messageWithSender = ":" + sender->getNick() + "!" + sender->username + "@localhost PRIVMSG " + this->getName() + " :" + message;

	for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it) {
		Client *member = *it;
		if (member != sender) {
			member->sendMessage(messageWithSender);
		}
	}
}

void	Channel::broadcastTopic( Client* sender){
	std::string topicMessage = RPL_TOPIC(sender->getNick(), sender->username, this->getName(), this->getTopic());
	for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it) {
		Client *member = *it;
		if (member != sender) {
			member->sendMessage(topicMessage);
		}
	}
}

void Channel::broadcastClientState(Client* client, std::string state) {
	std::string message;
	if(state == "join") {
		message = ":" + client->getNick() + "!user@host JOIN " + this->getName() + "\r\n";
	}
	else if(state == "part") {
		message = ":" + client->getNick() + "!user@host PART " + this->getName() + "\r\n";
		// For PART, only send to remaining members since parting user handles their own message
		for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
				(*it)->sendMessage(message);
		}
		return;
	}
	else if(state == "kick") {
		// For kicks, we need the source (kicker) and it should go to everyone including kicked user
		std::string kicker = "server";
		message = ":" + kicker + "!user@host KICK " + this->getName() + " " + client->getNick() + " :Kicked by " + kicker + "\r\n";
		// Send to all members including kicked user
		for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
			(*it)->sendMessage(message);
		}
		// Also send to kicked user
		client->sendMessage(message);
		return;
	}
	
	// For JOIN, broadcast to all members
	for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
		if (*it != client) {
			(*it)->sendMessage(message);
		}
	}
}


void	Channel::sendUsersList(Client *client){
	std::string message = ":serverhost 353 " + client->getNick() + " = " + this->getName() + " :";

	for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it) {
		Client *member = *it;
			if(this->isOperator(member))
				message += "@" + member->getNick() + " ";
			else
				message += member->getNick() + " ";
	}
	client->sendMessage(message);
	client->sendMessage(":serverhost 366 " + client->getNick() + " " + this->getName() + " :End of /NAMES list");
}


bool Channel::isMember(Client *client) {
	bool found = false;
	for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); it++) {
		if (strcasecmp((*it)->nickname.c_str(), client->nickname.c_str()) == 0) {
			found = true;
			return found;
		}
	}
	return found;
}

bool Channel::isOperator(Client *client) {
	bool found = false;
	for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); it++) {
		if (strcasecmp((*it)->nickname.c_str(), client->nickname.c_str()) == 0) {
			found = true;
			return found;
		}
	}
	return found;
}

bool Channel::isInvited(Client *client) {
	bool found = false;
	for (std::vector<Client*>::iterator it = people.begin(); it != people.end(); it++) {
		if (strcasecmp((*it)->nickname.c_str(), client->nickname.c_str()) == 0) {
			found = true;
			return found;
		}
	}
	return found;
}

void Channel::setTopic(const std::string& newTopic, const std::string& setter) {
	_topic = newTopic;
	_topicSetter = setter;
	_topicTimestamp = std::time(NULL); // Get current timestamp
}


std::string Channel::getTopicSetter() const {
	return _topicSetter;
}

std::time_t Channel::getTopicTimestamp() const {
	return _topicTimestamp;
}


void Channel::broadcastNotice(const std::string& message, Client* sender) {
	std::string noticeMessage = ":" + sender->getNick() + "!" + sender->username + "@localhost NOTICE " + this->getName() + " :" + message;

	for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); it++) {
		if(*it != sender)
			(*it) ->sendMessage(noticeMessage);
	}
}

void Channel::broadcastUserList() {
	std::vector<Client*>::iterator it = members.begin();
	for (; it != members.end(); it++) {
		sendUsersList(*it);
	}
}