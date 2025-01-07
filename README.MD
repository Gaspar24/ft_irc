#  42 Wolfsburg ft_irc
Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

## Getting Started

To get started with the project, follow these steps:

1. **Clone the repository**:
	```bash
	git clone https://github.com/yourusername/ft_irc.git
	cd ft_irc
	```

2. **Build the project**:
	```bash
	make
	```

3. **Run the server**:
	```bash
	./ircserver <port> <password>
	or
	make server
	```

Make sure you have all the necessary dependencies installed before building the project.

## Connecting to the Server

To connect to the server, you can use either the terminal with `nc` or a graphical IRC client like HexChat.

### Using Terminal with `nc`

1. Open a new terminal window.
2. Run the following command to connect to the server:
	```bash
	nc -C localhost 6667
	PASS <password>
	NICK <nickname>
	USER <username> 0 * <realname>
	```
3. You should now be connected to the IRC server.

### Using HexChat

1. Download and install HexChat from [here](https://hexchat.github.io/downloads.html).
2. Open HexChat and click on "Add" to create a new network.
3. Name the network (e.g., `ft_irc`).
4. Select the newly created network and click on "Edit".
5. Add a new server with the address `localhost/6667`.
6. Add PASS as method of authentication.
7.  Click "Close" and then "Connect" to join the server.

You should now be connected to the IRC server using HexChat.

## Available Commands

| Command          | Terminal Usage                          | HexChat Usage                          | Description                                      |
|------------------|-----------------------------------------|----------------------------------------|--------------------------------------------------|
| PASS             | `PASS <password>`                       | Entered on connection                  | Authenticate with server                         |
| NICK             | `NICK <nickname>`                       | `/nick <nickname>`                     | Set or change your nickname                             |
| USER             | `USER <username> 0 * <realname`         | Set on connection                      | Set username and realname                             |
| JOIN             | `JOIN <channel>`                        | `/join <channel>`                      | Join a specified channel                         |
| PRIVMSG          | `PRIVMSG <nickname/channel> <message>`  | `/msg <nickname/channel> <message>`    | Send a private message to a user or channel      |
| PART             | `PART #<channel>`                        | `/part #<channel>`                      | Leave a specified channel                        |
| QUIT             | `QUIT [message]`                        | `/quit [message]`                      | Disconnect from the server                        |
| KICK             | `KICK #<channel> <nickname> [message]`  | `/kick #<channel> <nickname> [message]`     | Remove a user from a channel                     |
| MODE             | `MODE #<channel> <<mode> [parameters]`    | `/mode #<channel> <mode> [parameters]`  | Change the mode of a channel             |
| TOPIC            | `TOPIC #<channel> [new topic]`           | `/topic #<channel> [new topic]`         | Get or set the topic of a channel                |
| WHO              | `WHO <nickname>`                        | `/who <nickname>`                      | List users in channel                     |
| INVITE           | `INVITE <nickname> <channel>`           | `/invite <nickname> <channel>`         | Invite a user to a channel                       |
| PING             | `PING <message>`                         | `/ping <message>`                       | Test the connection to the server                |
| PONG             | `PONG <ping_message>`                         | `/pong <ping_message>`                       | Reply to a PING command                          |



