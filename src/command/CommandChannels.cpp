# include "Commands.hpp"
# include "NumericMessages.hpp"
# include "Utils.hpp"
# include "InputParser.hpp"
#include "ChannelUtils.hpp"

void Command::handleJoin(Client& client, const std::string& args, std::map<std::string, Channel*>& channels) {
	std::istringstream ss(args);
	std::string channelName;
	ss >> channelName;
	channelName = trim(channelName);
	std::string passCode;
	ss >> passCode;
	passCode = trim(passCode);

	// Check if client registration is complete before joining a channel
	if (!client.isRegistered()) {
		sendCodeMessage(client, "451", client.getNick(), ":You have not registered");
		return;
	}
	if (channelName.empty() || channelName[0] != '#') {
        sendCodeMessage(client, "461", client.getNick(), "JOIN :Channel name must start with '#' or is empty");
		return;
	}



	// Check for long string. If found truncated
	channelName = Utils::truncateString(channelName);
	
	Channel* targetChannel = NULL;
	bool found = false;
	std::map<std::string, Channel*>::iterator it = channels.begin();
	for (; it != channels.end(); ++it) {
		if (it->first == channelName) {
			found = true;
			targetChannel = it->second;
		}
	}
	// if(!targetChannel->getPassword().empty() && passCode.empty()){
	// 	sendCodeMessage(client, "475", client.getNick(), "Private Channel, need a password.");
	// 	return;
	// }

	if (!found) {  //maybe create a function for this
		targetChannel = _server.getOrCreateChannel(channelName);
		targetChannel->addMember(&client);
		targetChannel->addOperator(&client);
		client.sendMessage(":" + client.getNick() + "!" + client.username + "@localhost" + " JOIN " + channelName + "\r\n");
		targetChannel->sendUsersList(&client);
		targetChannel->broadcastClientState(&client,"join");
		Utils::safePrint(client.getNick() + " has joined the channel: " + toStr(targetChannel->getName()));
		if (!targetChannel->getTopic().empty()) {
			client.sendMessage(":serverhost 332 " + client.getNick() + " " + channelName + " :" + targetChannel->getTopic());
			std::stringstream ss;
			ss << targetChannel->getTopicTimestamp();
			std::string topicTimestampStr = ss.str();
			// client.sendMessage(":serverhost 333 " + client.getNick() + " " + channelName + " " + targetChannel->getTopicSetter() + " " + std::to_string(targetChannel->getTopicTimestamp()));
			client.sendMessage(":serverhost 333 " + client.getNick() + " " + channelName + " " + targetChannel->getTopicSetter() + " " + topicTimestampStr);
		} else {
			client.sendMessage(":serverhost 332 " + client.getNick() + " " + channelName + " :\r\n");
		}
		// sendInformativeMessage(client,channelName + " was created","you are the operator of this channel.");
		Utils::safePrint(toStr(channelName) + " was created!");
		return;
	} else {
		// Check if the client is in channel;
		if(isMemberFct(targetChannel,client,channelName,channels)){  // here different format
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Already in channel";
			client.sendMessage(error);
			return;
		}
	
		// check if chaennel is full
		if (targetChannel->getMembers().size() >= static_cast<size_t>(targetChannel->getLimit())) {
			sendCodeMessage(client, "471", client.getNick(), "Channel is full.");
			return;
		}


		// check if invitation required
		if (targetChannel->getInviteStatus()) {
			bool invited = false;
			for (std::vector<Client*>::const_iterator it = targetChannel->getAllowedPeople().begin();
				 it != targetChannel->getAllowedPeople().end(); ++it) {
				if (*it == &client) {
					invited = true;
					break;
				}
			}
			if (!invited) {
				sendCodeMessage(client, "473", client.getNick(), "");
				return;
			}
		}
		if(!targetChannel->getPassword().empty()){
			// sendCodeMessage(client, "475", client.getNick(), "Private Channel, need a password.");
			if(passCode.empty())
			{
				sendCodeMessage(client, "475", client.getNick(), "Cannot join channel password field empty, Private channel need a password.");
				return;
			}
			else if(passCode != targetChannel->getPassword()){
				sendCodeMessage(client, "475", client.getNick(), "Cannot join channel (+k) - password wrong.");
				return;
			}
		}
	
	}
	// Join the channel	
	targetChannel->addMember(&client);
	// sendInformativeMessage(client,"Joined Channel",targetChannel->getName());
	client.sendMessage(":" + client.getNick() + "!" + client.username + "@localhost" + " JOIN " + channelName + "\r\n");
	targetChannel->sendUsersList(&client);
	targetChannel->broadcastClientState(&client,"join");
	Utils::safePrint(client.getNick() + " has joined the channel: " + toStr(targetChannel->getName()));
	if (!targetChannel->getTopic().empty()) {
   	 	client.sendMessage(":serverhost 332 " + client.getNick() + " " + channelName + " :" + targetChannel->getTopic());
		std::stringstream ss;
		ss << targetChannel->getTopicTimestamp();
		std::string topicTimestampStr = ss.str();
    	// client.sendMessage(":serverhost 333 " + client.getNick() + " " + channelName + " " + targetChannel->getTopicSetter() + " " + std::to_string(targetChannel->getTopicTimestamp()));
		client.sendMessage(":serverhost 333 " + client.getNick() + " " + channelName + " " + targetChannel->getTopicSetter() + " " + topicTimestampStr);
	} else {
		client.sendMessage(":serverhost 332 " + client.getNick() + " " + channelName + " :\r\n");
	}
}


void	Command::handleKick(Client& client, const std::string& args, std::map<std::string, Channel*>& channels){
	(void)channels;
	std::istringstream stream(args);
	std::string channelName;
	stream >> channelName;
	std::string target;
	stream >> target;

	if (channelName.empty() || channelName[0] != '#') {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Channel name must start with '#' or channel is empty.";
		client.sendMessage(error);
		return;
	}
	if (target.empty()) {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You need to chose a client.";
		client.sendMessage(error);
		return;

	}

	Channel *targetChannel = _server.getChannel(channelName);
	if (!targetChannel) {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Channel does not exist.";
		client.sendMessage(error);
		return;
	}
	bool isOperator = isOperatorFct(targetChannel,client);
	if (isOperator){
		bool found = false;
		for (unsigned long int i = 0; i < targetChannel->getMembers().size(); i++){
			if (targetChannel->getMembers()[i]->getNick() == target) {
				found = true;
				if(target == client.getNick()){
					std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You can t kick yourself dummy :))!. Use PART <#channel name> to exit channel.";
					client.sendMessage(error);
					return;
				}
				targetChannel->broadcastClientState(targetChannel->getMembers()[i], "kick");
				targetChannel->removeMember(targetChannel->getMembers()[i]);
				targetChannel->removePeople(targetChannel->getMembers()[i]);

				// Update NAMES list
				for (std::vector<Client*>::const_iterator it = targetChannel->getMembers().begin(); 
				it != targetChannel->getMembers().end(); ++it) {
				targetChannel->sendUsersList(*it);
			}
				// std::string response = "You got kicked from the channel:" + channelName + "\n";
				// send(targetChannel->getMembers()[i]->getFd(), response.c_str(), response.size() + 1, 0);
				Utils::safePrint(toStr(targetChannel->getMembers()[i]->getNick()) + " got kicked from: " + toStr(targetChannel->getName()));
				break;
			}
		}
		if (!found){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":User not found in the channel.";
			client.sendMessage(error);
		}
	}
	else {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You're not an operator in this channel";
		client.sendMessage(error);
	}
}

void	Command::handlePart(Client& client, const std::string& args, std::map<std::string, Channel*>& channels){
	(void)channels;
	std::istringstream stream(args);
	std::string channelName;
	stream >> channelName;
	if (channelName.empty() || channelName[0] != '#') {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Channel name must start with '#' or channel is empty.";
		client.sendMessage(error);		
		return;
	}
	Channel *targetChannel = _server.getChannel(channelName);
	if (!targetChannel) {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Channel dosen t exist.";
		client.sendMessage(error);
		return;
	}
	bool isOperator = isOperatorFct(targetChannel,client);
	bool found = false;
	for (unsigned long int i = 0; i < targetChannel->getMembers().size(); i++){
		if (targetChannel->getMembers()[i] == &client) {
			found = true;
			if (found) {
				targetChannel->broadcastClientState(&client, "part");

				// Send explicit part message to the leaving client
				client.sendMessage(":" + client.getNick() + "!" + client.username + "@localhost PART " + channelName + "\r\n");

				if(isOperator)	{
					targetChannel->removeOperator(&client);
				}
				targetChannel->removeMember(&client);
				targetChannel->removePeople(&client);
			}
			Utils::safePrint(client.getNick() + " has joined the channel: " + toStr(targetChannel->getName()));
			break;
		}
	}
	if (!found){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You're not an operator in this channel";
		client.sendMessage(error);
		return ;
	}
	if(targetChannel->getMembers().size() == 0){
		Utils::safePrint(toStr(targetChannel->getName()) + " was removed!");
		_server.removeChannel(targetChannel->getName());
		return;
	}
	if (targetChannel->getOperators().size() == 0 && targetChannel->getMembers().size() > 0) {
        Client* oldestMember = targetChannel->getMembers().front();
        targetChannel->addOperator(oldestMember);

		// Send MODE mesage to update operator status
		std::string promotionMessage = ":" + client.getNick() + "!" + client.username +"@localhost MODE " + targetChannel->getName() + " +o " + oldestMember->getNick();
        targetChannel->broadcast(promotionMessage, &client);
		// Update the names list
		targetChannel->broadcastUserList();
		// Broadcast informative message to all members
		targetChannel->broadcastNotice(oldestMember->getNick() + " is the new operator of the channel", &client);
		
		Utils::safePrint(toStr(oldestMember->getNick()) + " promoted to operator in channel: " + toStr(targetChannel->getName()));
	}

}


void	Command::handleTopic(Client& client, const std::string& args, std::map<std::string, Channel*>& channels){
	(void)channels;
	std::istringstream stream(args);
	std::string channelName;
	stream >> channelName;
	std::string topic;
	stream >> topic;
	if (channelName.empty() || channelName[0] != '#') {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Channel name must start with '#' or channel is empty.";
		client.sendMessage(error);
		return;
	}
	if(topic.empty()){
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Topic is empty.";
		client.sendMessage(error);
		return;
	}
	Channel *targetChannel = _server.getChannel(channelName);
	if (!targetChannel) {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Channel does not exist.";
		client.sendMessage(error);
		return;
	}
	bool isMember = isMemberFct(targetChannel,client,channelName, channels);
	if(isMember){
		if(targetChannel->getFlagTopic() == true){
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Topic restriction set to true, You need to be an operator..";
			client.sendMessage(error);
			bool isOperator = isOperatorFct(targetChannel,client);
			if(isOperator){
				if(targetChannel->getTopic() == topic){
					std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Topic already set to:" + topic;
					client.sendMessage(error);
					return;
				}
				targetChannel->setTopic(topic,client.getNick());
				std::string topicMsg = RPL_TOPIC(client.getNick(), client.username, targetChannel->getName(), targetChannel->getTopic());
				client.sendMessage(topicMsg);
				targetChannel->broadcastTopic(&client);
				Utils::safePrint(toStr(targetChannel->getName()) + " has the topic set to: " + targetChannel->getTopic());
			}
			else{
				std::string errorMsg = RPL_PRIVMSG(client.getNick(), client.username, channelName, ERR_CHANOPRIVSNEEDED(channelName));
				client.sendMessage(errorMsg);
				return ;
			}
		}
		else{
			if(targetChannel->getTopic() == topic){
					std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": Topic already set to:" + topic;
					client.sendMessage(error);
					return;
				}
			targetChannel->setTopic(topic,client.getNick());
			std::string topicMsg = RPL_TOPIC(client.getNick(), client.username, targetChannel->getName(), targetChannel->getTopic());
			client.sendMessage(topicMsg);
			targetChannel->broadcastTopic(&client);
			Utils::safePrint(toStr(targetChannel->getName()) + " has the topic set to: " + targetChannel->getTopic());
		}
	}
	else{
			std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":You are not a member of this channel";
			client.sendMessage(error);
			return ;
	}
	
}
		

void	Command::handleInvite(Client& client, const std::string& args, std::map<std::string, Channel*>& channels){
	(void)channels;
	std::istringstream stream(args);
	std::string targetNick, channelName;
	stream >> targetNick;
	stream >> channelName;
	if(targetNick.empty()){
		client.sendMessage(ERR_NEEDMOREPARAMS(std::string("INVITE")));
		return;
	}
	Client *targetClient = _server.getClientByNick(targetNick);
	if (!targetClient) {
		client.sendMessage(ERR_NOSUCKNICK(targetNick));
		return;
	}
	if (channelName.empty() || channelName[0] != '#') {
		client.sendMessage(ERR_NOSUCHCHANNEL(channelName));
		return;
	}

	Channel *targetChannel = _server.getChannel(channelName);
	if (!targetChannel) {
		client.sendMessage(ERR_NOSUCHCHANNEL(channelName));
		return;
	}
	bool isOperator = isOperatorFct(targetChannel, client);
	bool found = false;
	for (unsigned long int i = 0; i < targetChannel->getMembers().size(); i++){
		if (targetChannel->getMembers()[i] == &client) {
			found = true;
			if(isOperator){
				bool isMember = isMemberFct(targetChannel,*targetClient,channelName, channels);
				if(isMember){
					// std::string response = targetNick + " already in the channel.\n";
					// client.sendMessage(response);
					std::string messageWithSender = targetNick + " already in the channel";
					client.sendMessage(RPL_PRIVMSG(client.getNick(), client.username, targetChannel->getName(), messageWithSender));
					return ;
				}
				if (targetClient) {
					bool invitedAlready = false;
					std::vector<Client*> invited = targetChannel->getAllowedPeople();
					for (std::vector<Client*>::iterator It = invited.begin(); It != invited.end(); ++It) {
						if ((*It)->getNick() == targetNick) {
							invitedAlready = true;
							break;
						}
					}
					if(invitedAlready){
						std::string messageWithSender = targetNick + " already invited to the channel";
						client.sendMessage(RPL_PRIVMSG(client.getNick(), client.username, targetChannel->getName(), messageWithSender));
						return ;
					}
					targetChannel->addAllowedPeople(targetClient);

					// Send confirmation messages
					client.sendMessage(":serverhost 341 " + client.getNick() + " " + targetNick + " " + channelName);
					std::string response = ":" + client.getNick() + "!" + client.username + "@localhost INVITE " + targetNick + " " + channelName;
					targetClient->sendMessage(response);
				}
			} else {
					client.sendMessage(RPL_PRIVMSG(client.getNick(), client.username, targetChannel->getName(), ERR_CHANOPRIVSNEEDED(targetChannel->getName())));
			}
			break;
		}
	}
	if (!found){
		client.sendMessage(RPL_PRIVMSG(client.getNick(), client.username, targetChannel->getName(), ERR_NOTINCHANNEL(channelName)));
		return;
	}
}

void	Command::handleMode(Client& client, const std::string& args, std::map<std::string, Channel*>& channels){
	std::istringstream stream(args);
	std::string channelName;
	stream >> channelName;
	std:: string flag;
	stream >> flag;
	if (channelName.empty() || channelName[0] != '#') {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Channel name can t be empty or needs to start wit '#.";
		client.sendMessage(error);
		return;
	}
	Channel *targetChannel = _server.getChannel(channelName);
	if (!targetChannel) {
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ":Channel not found.";
		client.sendMessage(error);
		return;
	}
	if(flag.empty()){
		sendInformativeMessage(client, ":" + client.getNick() + "!" + client.username + "@localhost PRIVMSG " + targetChannel->getName() + " :" + "You can only use one of the appropriate flags: i,t,k,o,l.","");
		return;
	}

	bool isOperator = isOperatorFct(targetChannel,client);
	if(isOperator){
		if(flag == "k"){
			std::string passWord;
			stream >> passWord;
			if(!modePass(passWord,client,channelName,targetChannel)) return ;
		}
		else if(flag == "i")
		{
			std::string mode;
			stream >> mode;
			if(!modeInvite(mode,targetChannel,client,channelName)) return ;
		}
		else if(flag == "o"){ 
			std::string name;
			stream>>name;
			if(!modeOperator(name,channels,client,channelName,_server)) return ;
		}
		else if(flag == "l"){
			std::string limit;
			stream >> limit;
			if(!modeLimit(limit,client, targetChannel,channelName)) return ;
		}
		else if(flag == "t"){
			std::string mode;
			stream >> mode;
			if(!modeTopic(mode,client,targetChannel,channelName)) return ;
		}
		else{
			std::string messageWithSender = "You can only use one of the appropriate flags: i,t,k,o,l.";
			client.sendMessage(RPL_PRIVMSG(client.getNick(), client.username, targetChannel->getName(), messageWithSender));
		}
	}
	else{
		std::string error = ":" + client.username + "!user@host NOTICE " + channelName + " " + client.getNick() + ": You are not an operator in this channel." ;
		client.sendMessage(error);	}
}




//topic broadcast reversed -->done
//broadcast client join -->done
// broadcast client part -->done
//provide list with members in channel-->


// need to do the same when signal killed'
