"""
This script sends messages to an IRC channel using HexChat.

Functions:
	send_message(nick, message):
		Sends a message to the specified nick using HexChat.
		Args:
			nick (str): The nickname to send the message to.
			message (str): The message to send.
	
	read_users_from_file():
		Reads a list of users from a file.
		Returns:
			list: A list of user nicknames read from the file.

Usage:
	The script reads user nicknames from 'testScripts/users_created.txt' and sends a specified number of messages to each user.
	The messages are sent using HexChat with a delay of 1 second between each message.
"""
import random
import os
import time
import subprocess

#---------------------------------------------------------------

def send_message(nick, message):
	command = (
	f'flatpak run io.github.Hexchat --existing '
	f' --command="server irc"'
	f'--command="nick {nick}" '
	f'--command="msg {nick} {message}"'
	)
	result = subprocess.run(command, shell=True, capture_output=True, text=True)
	print("Output:", result.stdout)
	print("Error:", result.stderr)
	print("Return Code:", result.returncode)

#---------------------------------------------------------------
def read_users_from_file():
	USERS = "testScripts/users_created.txt"  # File to store created users
	users = []
	if os.path.exists(USERS):
		with open(USERS, "r") as file:
			users = [line.strip() for line in file.readlines()]
	return users
#---------------------------------------------------------------
# Example context IDs for the user windows
nicks = read_users_from_file()  # Replace with actual context IDs of your user windows

CHANNEL = "#Channel0"
NUM_MESSAGES = 2  # Number of messages to send

for nick in nicks:
	for i in range(1, NUM_MESSAGES + 1):
		message = f"{i} Hello from {nick}!"
		print(f"Sending message from Nick: {nick}")
		send_message(nick, message)
		time.sleep(1) 