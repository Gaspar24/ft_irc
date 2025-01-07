#ifndef NUMERIC_MESSAGES_HPP
# define NUMERIC_MESSAGES_HPP


# define USERLEN 100

/* ERROR MESSAGES */
# define ERR_PASSWDMISMATCH		 ":serverhost 464 :Password incorrect\r\n"
# define ERR_NICKNAMEINUSE(nick) ":serverhost 433 * " + nick + " :Nickname is already in use\r\n"
# define ERR_UNKNOWNCOMMAND(command) ":serverhost 421 * " + command + " :Unknown command\r\n"
# define ERR_NEEDMOREPARAMS(user) ":serverhost 461 " + user + " :Not enough parameters\r\n"
# define ERR_ALREADYREGISTERED ":serverhost 462 :You may not reregister\r\n"
# define ERROR(reason) "Error: " + reason + "\r\n"
# define ERR_NOOPERHOST " server 491 * :No O-lines for your host\r\n"
# define ERR_NOTREGISTERED " server 451 * :You have not registered\r\n"
# define ERR_NOSUCKNICK(nickname) " server 401 * (" + nickname + ") :No such nickname \r\n"
# define ERR_NOSUCHCHANNEL(channel) " server 403 * (" + channel + ") :No such channel \r\n"
# define ERR_CANNOTSENDTOCHAN(channel) ":serverhost 404 " + channel + ":Cannot send to channel\r\n"
# define ERR_CHANOPRIVSNEEDED(channel) ":serverhost 482 " + channel + ":You're not a channel operator\r\n"
# define ERR_ALREADYINCHANNEL(nick, targetNick, channel) ":serverhost 443 " + nick + " " + targetNick + " " + channel + " :is already on channel"
# define ERR_NOTINCHANNEL(channel) ":serverhost 442 " + channel + " :You're not on that channel"

/* REPLY MESSAGES */
# define RPL_NICKCHANGE(oldNick, user, host, newNick)  ":" + oldNick + "!" + user + "@" + host + " NICK :" + newNick + "\r\n"
# define RPL_EMPTYCAPLIST(host) ":" + host + " CAP " + " LS :\r\n"
# define RPL_QUIT(nickname, username, reason) ":" + nickname + "!" + username + "@localhost QUIT :Quit: " + reason + "\r\n"
# define RPL_WELCOME(nickname) "Welcome to our IRC network " + nickname + " !\r\n"
# define RPL_YOUREOPER(nickname) ":serverhost 381 " + nickname + ":You are now an IRC operator \r\n"
# define RPL_WHOREPLY(channel, nick, user, realname) ":serverhost 352 " + nick + " " + channel + " " + user + " @localhost serverhost " + nick + " H :0 " + realname
# define RPL_ENDOFWHO(target) ":serverhost 315 " + target + " " + target + " :End of WHO list"
# define RPL_PRIVMSG(nick, username, targetname, message) ":" + nick + "!" + username + "@localhost PRIVMSG " + targetname + " :" + message
# define RPL_TOPIC(nick, username, channel, topic) ":" + nick + "!" + username + "@localhost TOPIC " + channel + " :" + topic

#endif
