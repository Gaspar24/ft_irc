
#ifndef CHANNEL_UTILS_HPP
#define CHANNEL_UTILS_HPP

# include "Commands.hpp"
# include "Utils.hpp"

bool	modePass(std::string passWord, Client& client, std::string channelName,Channel* targetChannel);
bool	modeInvite(std::string mode, Channel *targetChannel, Client &client, std::string channelName);
bool 	modeOperator(std::string name, std::map<std::string, Channel*> &channels,Client &client, std::string channelName, Server &_server);
bool	modeLimit(std::string limit, Client &client,Channel *targetChannel, std::string channelName);
bool 	modeTopic(std::string mode, Client &client, Channel *targetChammel, std::string channelName);

bool	isMemberFct(Channel *targetChannel, Client &client, std::string channelName, std::map<std::string, Channel*> &channels);
bool	isOperatorFct(Channel *targetChannel, Client &client);

void sendInformativeMessage(Client& client, const std::string& baseMessage, const std::string& details = "");
void sendCodeMessage(Client& client, const std::string& code, const std::string& target, const std::string& message);


#endif