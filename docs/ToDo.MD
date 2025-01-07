# Vitoria's List

## Makefile

- [x] setup rules to work with branches
- [x] change git message to include current @git user

## DOCS
- [x] Fix rules in gitTools/Makefile


## Server

- [x] change nonblocking connection to check flags 0 and use return value from fcntl
- [x] change from read() to recv()
- [x] Check if ip and port are restricted values or allowed, set up warnings?
- [ ] If connection is lost with all clients, clean screen?
- [ ] when debug=true, keep log with connections
- [ ] check with helgrind for threads and file descriptors

```bash
	For setting up a local network, such as an IRC server, you can use any available port that is not already in use by another service. Commonly used ports for IRC servers are:

- **6667**: The default port for IRC.
- **6660-6669**: A range of ports often used for IRC.
- **7000-7005**: Another range of ports sometimes used for IRC.
```
```bash
	For setting up a local network, such as an IRC server, you can use any available port that is not already in use by another service. Commonly used ports for IRC servers are:

- **6667**: The default port for IRC.
- **6660-6669**: A range of ports often used for IRC.
- **7000-7005**: Another range of ports sometimes used for IRC.
```
## Client

- [x] Check python testHexChaty on server message
	[ ] Delete server info script and automatic message
- [ ] Check if client connection is a new file descriptor when joining a channel or making new connection, is 

## Channels

- [ ] Test one user join channel and kill server
- [ ] Test multiple users join channel and kill server
- [ ] Broadcast message when new client joins to everyone
- [ ] Topic message is shown in  old clients but not in new client !!!!
- [x] User joint channel, another two did the same, send message to all, then first user was terminated, try to sent a message broadcasted from another client and got this error
- [x] Broadcast when client termnates or leave channel

# NOTES

In IRC, server replies are identified by numeric codes. These codes are used to convey various types of information, such as connection status, errors, and responses to commands. Here are some common numeric reply codes that you might find useful when hooking server events in HexChat:

### Connection and Registration Replies
- [x] **001**: RPL_WELCOME - Welcome message after successful login.
- [ ] **002**: RPL_YOURHOST - Information about the server.
- [ ] **003**: RPL_CREATED - Server creation date.
- [ ] **004**: RPL_MYINFO - Server information.
- [ ] **005**: RPL_ISUPPORT - Supported features.

### Channel Replies
- [ ] **324**: RPL_CHANNELMODEIS - Channel modes.
- [ ] **332**: RPL_TOPIC - Channel topic.
- [ ] **333**: RPL_TOPICWHOTIME - Topic setter and time.
- [ ] **353**: RPL_NAMREPLY - List of users in a channel.
- [ ] **366**: RPL_ENDOFNAMES - End of NAMES list.

### User Replies
- [ ] **301**: RPL_AWAY - User is marked as away.
- [ ] **311**: RPL_WHOISUSER - WHOIS user information.
- [ ] **312**: RPL_WHOISSERVER - WHOIS server information.
- [ ] **313**: RPL_WHOISOPERATOR - WHOIS operator information.
- [ ] **317**: RPL_WHOISIDLE - WHOIS idle time.
- [ ] **318**: RPL_ENDOFWHOIS - End of WHOIS list.
- [ ] **319**: RPL_WHOISCHANNELS - Channels the user is in.

### Error Replies
- [ ] **401**: ERR_NOSUCHNICK - No such nick/channel.
- [ ] **403**: ERR_NOSUCHCHANNEL - No such channel.
- [ ] **404**: ERR_CANNOTSENDTOCHAN - Cannot send to channel.
- [ ] **421**: ERR_UNKNOWNCOMMAND - Unknown command.
- [ ] **433**: ERR_NICKNAMEINUSE - Nickname is already in use.

## EVAL

Basic Checks
- [x] Makefile and Compilation:

	Ensure the project compiles correctly using the Makefile.

	Verify the executable is named ircserv.

- [x] Single poll() Call:

	Confirm there is only one poll() call in the code, located in Server::run.

- [x] poll() Before accept, read/recv, write/send:

	Verify poll() is called before accept, recv, and send in the respective functions.

- [!] fcntl() Usage: ATENTION!!!!

	Ensure fcntl(fd, F_SETFL, O_NONBLOCK) is used correctly in Server::setNonBlocking.

### Networking Specials

- [x] Partial Commands:

	Use nc to send partial commands and verify the server handles them correctly.

- [x] Unexpected Client Kill:

	Kill a client unexpectedly and check if the server remains operational for other connections and new clients.

- [x] Half Command Sent:

	Use nc to send half a command and verify the server does not enter an odd state or get blocked.

- [ ] Stop Client and Flood Channel:  ATENTION!!

	Stop a client connected to a channel and flood the channel using another client. Ensure the server does not hang and processes stored commands when the client resumes.
	* ! Error happend when 3 users were in channel and operator disconected
		- [ ] Check if is solved when integrated main branch

### Client Commands Basic

- [x] Authentication, Nickname, Username, Join Channel:

	Test these functionalities using both nc and the reference IRC client.

- [x] Private Messages (PRIVMSG):

	Verify PRIVMSG functionality with different parameters.

### Client Commands Channel Operator

- [ ] Regular User Privileges:

	Ensure regular users do not have channel operator privileges.

- [ ] Channel Operator Commands:

	Test all channel operator commands using both nc and the reference IRC client.

### Memory Leaks

- [! ] Memory Leak Check:

	Check for memory leaks and data racings during various operations.


### Additional Tests

- [ ] File Transfer (if implemented):

	Test file transfer functionality with the reference IRC client.

- [ ] IRC Bot (if implemented):

	Test the IRC bot functionality.
