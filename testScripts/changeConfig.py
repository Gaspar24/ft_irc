"""
This script modifies the HexChat IRC client configuration files to set up a new user with a randomly generated unique number.
Modules:
	random: Generates random numbers.
	os: Provides a way of using operating system dependent functionality.
	time: Provides various time-related functions.
Variables:
	UNIQUE_NUMBER: A randomly generated unique number between 1000 and 9999.
	NICK: The nickname for the user, based on UNIQUE_NUMBER.
	USERNAME: The username for the user, based on UNIQUE_NUMBER.
	REALNAME: The real name for the user, based on UNIQUE_NUMBER.
	SERVER: The IRC server address.
	PORT: The port number for the IRC server.
	USERS: The path to the file where created users are stored.
	servlist_path: The path to the HexChat server list configuration file.
	hexchat_config_path: The path to the HexChat main configuration file.
Functions:
	None
Script Functionality:
	1. Generates a random unique number and creates user data based on it.
	2. Prints the server, nickname, and username to the console.
	3. Appends the generated nickname to a file storing created users.
	4. Reads the HexChat server list configuration file and checks if the network configuration already exists.
	5. Writes the existing lines back to the server list configuration file and appends the new network configuration if it doesn't exist.
	6. Reads the HexChat main configuration file and modifies it to set the generated nickname and username.
	7. Prints a success message to the console.
"""
import os
import time
import random

BLUE = '\033[94m'
CYAN = '\033[96m'
MAG = '\033[95m'
GREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
ENDC = '\033[0m'
BOLD = '\033[1m'
UNDERLINE = '\033[4m'


# Generate random user data with the same unique number
UNIQUE_NUMBER = random.randint(0, 50)
NICK = f"user{UNIQUE_NUMBER}"
USERNAME = f"user{UNIQUE_NUMBER}"
REALNAME = f"Real{UNIQUE_NUMBER}"
SERVER = "localhost"
PORT = 6667  # Non-SSL port
# NETWORK_PREFIX = "network"
# NETWORK = f"{NETWORK_PREFIX}{UNIQUE_NUMBER}"

print(f"server: {MAG}irc{ENDC}")
print(f"Nick: {BLUE}{NICK}{ENDC}")
print(f"Username: {BLUE}{UNDERLINE}{USERNAME}{ENDC}")
# print(f"Generated Realname: {GREEN}{UNDERLINE}{REALNAME}{ENDC}")
#-------------------------------------------------------------------
USERS= "testScripts/users_created.txt"
with open(USERS, "a") as file:
	file.write(f"{NICK}\n")
#-------------------------------------------------------------------

servlist_path = os.path.expanduser("~/.var/app/io.github.Hexchat/config/hexchat/servlist.conf")


print(f"Writing to servlist.conf: {servlist_path}")
with open(servlist_path, "r") as servlist_file:
	lines = servlist_file.readlines()
network_exists = any(line.startswith(f"N=irc\n") for line in lines)
# Write the existing lines back to the file and append the new network configuration if it doesn't exist
with open(servlist_path, "w") as servlist_file:
	for line in lines:
		servlist_file.write(line)
	if not network_exists:
		servlist_file.write(f"\nN=irc\n")
		servlist_file.write(f"E=UTF-8 (Unicode)\n")
		servlist_file.write(f"F=115\n")
		servlist_file.write(f"D=0\n")
		servlist_file.write(f"S={SERVER}/{PORT}\n")

#-------------------------------------------------------------------
hexchat_config_path = os.path.expanduser("~/.var/app/io.github.Hexchat/config/hexchat/hexchat.conf")
print(f"Modifying hexchat.conf: {hexchat_config_path}")
with open(hexchat_config_path, "r") as hexchat_config_file:
	lines = hexchat_config_file.readlines()
	
with open(hexchat_config_path, "w") as hexchat_config_file:
	for line in lines:
		if line.startswith(f"irc_nick1 ="):
			hexchat_config_file.write(f"irc_nick1 = {NICK}\n")
		elif line.startswith(f"irc_nick2 ="):
			hexchat_config_file.write(f"irc_nick2 = {NICK}_\n")
		elif line.startswith(f"irc_nick3 ="):
			hexchat_config_file.write(f"irc_nick3 = {NICK}__\n")
		elif line.startswith(f"irc_user_name ="):
			hexchat_config_file.write(f"irc_user_name = {USERNAME}\n")
		else:
			hexchat_config_file.write(line)
	
print(GREEN,BOLD, UNDERLINE, "Configuration modified successfully.\n", ENDC)
time.sleep(1)

