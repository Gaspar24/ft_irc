import hexchat
import re

HEXCHAT_COLORS = {
    '0': '\00301',  # Black
    '1': '\00304',  # Red
    '2': '\00303',  # Green
    '3': '\00308',  # Yellow
    '4': '\00302',  # Blue
    '5': '\00305',  # Brown
    '6': '\00306',  # Purple
    '7': '\00307',  # Orange
    '8': '\00314',  # Grey
    '9': '\00304',  # Bright Red
    '10': '\00309', # Bright Green
    '11': '\00308', # Bright Yellow
    '12': '\00312', # Bright Blue
    '13': '\00313', # Bright Magenta
    '14': '\00311', # Bright Cyan
}

LIGHT_GREY = "\00315"
ENDC = "\003"
ANSI_TO_HEXCHAT = {
    **{str(i): HEXCHAT_COLORS[str(i % 15)] for i in range(256)}
}

# Formatting codes
HEXCHAT_FORMATTING = {
    '1': '\002',  # Bold
    '4': '\037',  # Underline
    '7': '\026',  # Reverse
    '0': '\017',  # Reset
}
__module_name__ = "On Handshake"
__module_version__ = "1.0"
__module_description__ = "Prints a message when the handshake is established"

# Regular expression to match ANSI color codes
# ansi_escape = re.compile(r'\x1B\[(\d+)(;\d+)*m|\x1B\[(\d+);(\d+);(\d+);(\d+);(\d+)m|\x1B\[(\d+);(\d+);(\d+);(\d+);(\d+);(\d+)m')
ansi_escape = re.compile(r'\x1B\[[0-9;]*[mK]')
# ansi_escape = re.compile(r'\x1B\[(\d+)(;\d+)*m')
connection_established = False
message = []
#--------------------------------------------------------------------------------
def send_message(nick):
	print()
	hexchat.command(f"msg {nick} {nick}test")
	return False
#--------------------------------------------------------------------------------
def hexChatMsg():
	context = hexchat.get_context()
	if context is not None:
		network = context.get_info("network")
		server = context.get_info("server")
		nick = context.get_info("nick")
		user = context.get_info("user")
		realname = context.get_info("realname")
		context_id = context.get_info("contextid")
		
		hexchat.prnt(f"\n{LIGHT_GREY}Connected to network: {network}{ENDC}")
		hexchat.prnt(f"{LIGHT_GREY}Server address: {server}{ENDC}")
		hexchat.prnt(f"{LIGHT_GREY}Nick: {nick}{ENDC}")
		hexchat.prnt(f"{LIGHT_GREY}User name: {user}{ENDC}")
		hexchat.prnt(f"{LIGHT_GREY}Real name: {realname}{ENDC}")
		hexchat.prnt(f"{LIGHT_GREY}Context ID: {context_id}{ENDC}")
		hexchat.hook_timer(1000, send_message, nick)
	else:
		hexchat.prnt(f"{LIGHT_GREY}Failed to get current context{ENDC}")
	return False
#--------------------------------------------------------------------------------
def replace_ansi(match):
	codes = match.group(0).split(';')
	hexchat_color = HEXCHAT_COLORS.get(codes[-1].strip('m'), '')
	return hexchat_color

def translate_ansi_to_hexchat(message):
    def replace_ansi0(match):
        codes = match.group(0).strip('\x1B[').strip('m').split(';')
        hexchat_color = ''
        for code in codes:
            if code in ANSI_TO_HEXCHAT:
                hexchat_color = ANSI_TO_HEXCHAT[code]
            elif code in HEXCHAT_FORMATTING:
                hexchat_color += HEXCHAT_FORMATTING[code]
        return hexchat_color
    return ansi_escape.sub(replace_ansi0, message) + '\003'

def toStr(lst):
	return '\n'.join(lst) + '\n'

def on_handshake(word, word_eol, userdata):
	global connection_established
	global message
	# hexchat.prnt(f"eol: {len(word_eol)} | word: {len(word)} )")
	if not word_eol:
		message.append("\n")
	if word_eol:
		message.append(word_eol[0])
		line = translate_ansi_to_hexchat(word_eol[0])
		# line = ansi_escape.sub('', word_eol[0])
		if "FT_IRC" in word:
			if not connection_established:
				hexchat.prnt(f"{LIGHT_GREY}------------- HANDSHAKE WITH SERVER -------------{ENDC}")
				connection_established = True
			match = ansi_escape.search(message[0])
			if match:
				color = replace_ansi(match)
				for msg in message:
					newMsg = ansi_escape.sub('', msg)
					hexchat.prnt(f"{color}{msg}\003")
			# hexChatMsg()
		elif connection_established:
			hexchat.prnt(f"{line}")
			# try:
			# 	message = message.encode('ascii', 'ignore').decode('ascii')
			# except UnicodeEncodeError:
			# 	message = message  # Fallback to original message if encoding fails
			# char_count = len(message)
			# print("{} (len:{})".format(message, char_count))
			# hexchat.prnt(f"{message} ({char_count} |eol: {len(word_eol)} | word: {len(word)} )")

	return hexchat.EAT_HEXCHAT

def process_ansi_color(word, word_eol, userdata):
	# Translate ANSI color codes to HexChat color codes
	# if not word_eol:
	# 	message.append("\n")
	if word:
		message = translate_ansi_to_hexchat(word[0])
		# Print the message with HexChat color codes
		hexchat.prnt(f"{message}")
	return hexchat.EAT_HEXCHAT

# Hook the print event for server messages
# hexchat.hook_print("Server Text", process_ansi_color)
# hexchat.hook_print("Channel Message", process_ansi_color)
# hexchat.hook_print("Notice", process_ansi_color)
# hexchat.hook_print("Private Message", process_ansi_color)


# Hook the print event for channel messages
# hexchat.hook_print("Channel Message", process_ansi_color)

#--------------------------------------------------------------------------------
# Hook into server messages
# hexchat.hook_server("Server Text", on_handshake)
hexchat.hook_print("Server Text", process_ansi_color)
# hexchat.hook_print("Server Text", on_handshake)
# hexchat.hook_server("Server Text", process_ansi_color)
# hexchat.hook_server("RAW LINE", process_ansi_color)
# hexchat.hook_print("Channel Message", process_ansi_color)
# hexchat.hook_print("Notice", process_ansi_color)
# hexchat.hook_print("Private Message", process_ansi_color)
# hexchat.hook_print("Server Text", on_handshake)
#--------------------------------------------------------------------------------
# print(__module_name__, "version", __module_version__, "loaded.")
# hexChatMsg()

