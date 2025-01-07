# include "Commands.hpp"
# include "NumericMessages.hpp"
# include "Utils.hpp"
# include "InputParser.hpp"


void sendCodeMessage(Client& client, const std::string& code, const std::string& target, const std::string& message) {
    std::string formattedMessage = ":serverhost " + code + " " + target + " :" + message;
    client.sendMessage(formattedMessage);
}



bool	modePass(std::string passWord,Client& client, std::string channelName,Channel* targetChannel){
	if(passWord.empty()){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Password field empty." ;
		client.sendMessage(error);
		return false;
	}
	if(targetChannel->getPassword() == passWord){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Password already set to:" + passWord ;
		client.sendMessage(error);
		return false;
	}
	targetChannel->setPassword(passWord);
	std::string error = ":" + client.username + "!user@host MODE " + channelName + " +k " + client.getNick() + ":Password for the channel: " + channelName +" set to: " +passWord;
	client.sendMessage(error);
	targetChannel->broadcastNotice("The operator has set password for this channel.",&client);
	return true;
}

bool	modeInvite(std::string mode, Channel *targetChannel, Client &client, std::string channelName){
	if(mode.empty()){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You need to add a mode after the flag.";
		client.sendMessage(error);
		return false;
	}
	if(mode == "set"){
		if(targetChannel->getInviteStatus()){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Already set to invitation only..";
			client.sendMessage(error);
			return false;
		}
		targetChannel->setInviteStatus(true);
		std::string error = ":" + client.username + "!user@host MODE " + channelName + " +i " + client.getNick() + ":Channel mode set to invite only.";
		client.sendMessage(error);
		targetChannel->broadcastNotice("The operator has set channel mode for invite only.",&client);

	}
	else if (mode == "remove")
	{
		if(!targetChannel->getInviteStatus()){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Invitation only already removed.";
			client.sendMessage(error);
			return false;
		}
		targetChannel->setInviteStatus(false);
		std::string error = ":" + client.username + "!user@host MODE " + channelName + " -i " + client.getNick() + ":Invitation only  mode removed.";
		client.sendMessage(error);
		targetChannel->broadcastNotice("The operator has removed the invite only mode.",&client);

		
	}
	else {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You can only use set or remove.";
		client.sendMessage(error);
		return false; // added
	}
	return true;
}

bool modeOperator(std::string name, std::map<std::string, Channel*> &channels,Client &client, std::string channelName, Server &_server){
if(name.empty()){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":No client name given.";
		client.sendMessage(error);
		return false ;
	}
	bool isMember = false;
	Channel* targetChannel = NULL;
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->first == channelName) {
			targetChannel = it->second;
			std::vector<Client*> members = targetChannel->getMembers();
			for (std::vector<Client*>::iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt) {
				if ((*memberIt)->getNick() == name) {
					isMember = true;
					break;
				}
			}
			break;
		}
	}
	if(isMember){
		if(client.getNick() == name){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You are already an operator dummy.! :))";
			client.sendMessage(error);
			return false;
		}
		bool isAlreadyOperator = false;
		std::vector<Client*> operators = targetChannel->getOperators();
		for (std::vector<Client*>::iterator It = operators.begin(); It != operators.end(); ++It) {
			if ((*It)->getNick() == name) {
				isAlreadyOperator = true;
				break;
			}
		}
		if(isAlreadyOperator)
		{
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":" + name + " is already an operator.";
			client.sendMessage(error);
			return false;
		}
		Client *clientTarget = _server.getClientByNick(name);
		std::string error = ":" + client.username + "!user@host MODE " + channelName + " +o " + clientTarget->getNick() ;
		client.sendMessage(error);
		targetChannel->addOperator(clientTarget);
		targetChannel->broadcastNotice(client.getNick() + " gave operator priviliges to :" + clientTarget->getNick(),&client);

		// targetChannel->sendUsersList(&client);

		targetChannel->broadcastUserList();
		std::string message = ":" + client.username + "!user@host MODE " + channelName + " +o " + clientTarget->getNick();
		clientTarget->sendMessage(message);
		std::cout << "new operator added in the chanel : " <<  name << std::endl;
	}
	else{
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": " + name + " not found in the channel.";
		client.sendMessage(error);
	}
	return true;
}

bool	modeLimit(std::string limit, Client &client,Channel *targetChannel, std::string channelName){
	if(limit.empty()){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":No limit given.";
		client.sendMessage(error);
		return false;
	}
	if(!isNumber(limit)){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Limit must be anumber.";
		client.sendMessage(error);
		return false;
	}

	long int nb = modAtoi(limit);
	
	if(nb >1024 )
	{
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You can t insert a number bigger than max numbers of fd's, 1024.";
		client.sendMessage(error);
		return false;
	}
	else if(nb <= 0)
	{
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You can t insert a negative/null number.";
		client.sendMessage(error);
		return false;
	}
	if ( static_cast<size_t>(nb) < targetChannel->getMembers().size()) {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You can t insert a limit smaller than the number of clients already in the channel.";
		client.sendMessage(error);
		return false;
	}
	if(targetChannel->getLimit() == nb){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Limit already set to: " + limit;
		client.sendMessage(error);
		return false;
	}
	
	targetChannel->setLimit(static_cast<int>(nb));
	std::string error = ":" + client.username + "!user@host MODE " + channelName + " +l " + client.getNick() + ":Limit clients set to: " + limit;
	client.sendMessage(error);
	targetChannel->broadcastNotice("The operator has set the limit for the channel members.",&client);
	return true;
}

bool modeTopic(std::string mode, Client &client, Channel *targetChannel, std::string channelName){
	if(mode.empty()){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":No mode given.";
		client.sendMessage(error);
		return false; 
	}
	
	if(mode == "set")
	{
		if(targetChannel->getFlagTopic()){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Topic restriction is already set.";
			client.sendMessage(error);
			return false;
		}
		std::string error = ":" + client.username + "!user@host MODE " + channelName + " +t " + client.getNick() + ":Topic restriction is on.";
		client.sendMessage(error);
		targetChannel->setFlagTopic(true);
		targetChannel->broadcastNotice("The operator has set restriction for the channel topic.",&client);

	}

	else if(mode == "remove") {
		if(!targetChannel->getFlagTopic()){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Topic restriction is already removed.";
			client.sendMessage(error);
			return false ;
		}
		std::string error = ":" + client.username + "!user@host MODE " + channelName + " -t " + client.getNick() + ":You removed topic restriction.";
		client.sendMessage(error);
		targetChannel->setFlagTopic(false);
			targetChannel->broadcastNotice("The operator has removed restriction for the channel topic.",&client);

	}
	else {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You can only use set or remove.";
		client.sendMessage(error);
		return false;
	}
	return true;
}


bool	isMemberFct(Channel *targetChannel, Client &client, std::string channelName, std::map<std::string, Channel*> &channels){
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->first == channelName) {
			targetChannel = it->second;
			std::vector<Client*> members = targetChannel->getMembers(); // Get the members vector
			for (std::vector<Client*>::iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt) {
				if (*memberIt == &client) {
					return true;
				}
			}
			break;
		}
	}
	return false;
}

bool	isOperatorFct(Channel *targetChannel, Client &client){
	for (unsigned long int i = 0; i < targetChannel->getOperators().size(); i++){
		if (targetChannel->getOperators()[i] == &client) {
			return true;
		}
	}
	return false;
}


void sendInformativeMessage(Client& client, const std::string& baseMessage, const std::string& details = "") {
    std::string errorMessage = baseMessage;
    if (!details.empty()) {
        errorMessage += ": " + details;
    }
    errorMessage += "\n";
    client.sendMessage(errorMessage);
}


