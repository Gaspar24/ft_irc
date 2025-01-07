"""
This script is a HexChat plugin that manages HexChat windows.

Modules:
	hexchat: The HexChat API module.

Functions:
	getContext():
		Retrieves and prints the current HexChat context ID.
		Returns:
			context_id (str): The ID of the current HexChat context.

	my_command_callback(word, word_eol, userdata):
		Callback function for the custom HexChat command "MYCOMMAND".
		Args:
			word (list): List of words in the command.
			word_eol (list): List of words from the end of the line.
			userdata (any): User data passed to the callback.
		Returns:
			hexchat.EAT_ALL: Indicates that the command has been fully handled.

	open_new_window():
		Opens a new HexChat window and hooks the custom command "MYCOMMAND".
"""
import hexchat
import time
import threading

WHITE = "\00300"
BLACK = "\00301"
BLUE = "\00302"
GREEN = "\00303"
RED = "\00304"
BROWN = "\00305"
PURPLE = "\00306"
ORANGE = "\00307"
YELLOW = "\00308"
LIGHT_GREEN = "\00309"
TEAL = "\00310"
LIGHT_CYAN = "\00311"
LIGHT_BLUE = "\00312"
PINK = "\00313"
GREY = "\00314"
LIGHT_GREY = "\00315"
ENDC = "\003"

__module_name__ = "StartConnection"
__module_version__ = "1.0"
__module_description__ = "Manages HexChat conection to server"

def getContext():
	context = hexchat.get_context()
	if context is not None:
		network = context.get_info("network")
		server = context.get_info("server")
		nick = context.get_info("nick")
		user = context.get_info("user")
		realname = context.get_info("realname")
		context_id = context.get_info("contextid")
		
		hexchat.prnt(f"{GREEN}Connected to network: {network}{ENDC}")
		hexchat.prnt(f"{GREEN}Server address: {server}{ENDC}")
		hexchat.prnt(f"{GREEN}Nick: {nick}{ENDC}")
		hexchat.prnt(f"{GREEN}User name: {user}{ENDC}")
		hexchat.prnt(f"{GREEN}Real name: {realname}{ENDC}")
		hexchat.prnt(f"{GREEN}Context ID: {context_id}{ENDC}")
		return context
	else:
		print("Failed to get current context")
		hexchat.prnt("Failed to get current context")
		return None

def send_message(nick):
	hexchat.command(f"msg {nick} {nick}test")
	return hexchat.EAT_ALL

# def cmd(word, word_eol, userdata):
	# hexchat.prnt(f"{BLUE}cmd was called!{ENDC}")
	# hexchat.prnt(f"{BLUE}word: {word}{ENDC}")
	# hexchat.prnt(f"{BLUE}word_eol: {word_eol}{ENDC}")
	# context = hexchat.get_context()
	# hexchat.prnt(RED + str(context) + ENDC)
	# if context is not None:
	# 	print(f"Current Context ID: {context_id}")
	# 	network = context.get_info("network")
	# 	server = context.get_info("server")
	# 	nick = context.get_info("nick")
	# 	user = context.get_info("user")
	# 	realname = context.get_info("realname")
	# 	context_id = context.get_info("contextid")
		
	# 	hexchat.prnt(f"{GREEN}Connected to network: {network}{ENDC}")
	# 	hexchat.prnt(f"{GREEN}Server address: {server}{ENDC}")
	# 	hexchat.prnt(f"{GREEN}Nick: {nick}{ENDC}")
	# 	hexchat.prnt(f"{GREEN}User name: {user}{ENDC}")
	# 	hexchat.prnt(f"{GREEN}Real name: {realname}{ENDC}")
	# 	hexchat.prnt(f"{GREEN}Context ID: {context_id}{ENDC}")
	# else:
	# 	print(RED + "Failed to get current context" + ENDC)

	# return hexchat.EAT_ALL



def runServer():
	# hexchat.hook_command("cmd", cmd)
	# hexchat.command("set text_font Ubuntu Mono 9")
	#Get info of current session
	# hexchat.command(f"set irc_user_name {nick}")
	# hexchat.command(f"set irc_real_name Real{nick}")

	# Connect to server command
	hexchat.command("server irc")

runServer()
