#!/bin/bash

# Configuration
SERVER="localhost"
PORT=6667
PASSWORD="42"
# Lists of possible values
NICKS=("Vicki" "Memo" "Marian" "Test")
USERS=("Victoria" "Guillermo" "Marian" "Test")
REALNAMES=("VictoriaL" "GuillermoC" "MarianS" "TestR")

# Select random values
index=$((RANDOM % ${#NICKS[@]}))
NICK=${NICKS[$index]}
USER=${USERS[$index]}
REALNAME=${REALNAMES[$index]}
CHANNEL="#test"
MESSAGE="Hello, IRC!"

is_server_running()
{
	# timeout 0.1 bash -c "</dev/tcp/$SERVER/$PORT" &>/dev/null
	pgrep ircserv
	return $?
}

# Set the trap to catch SIGINT (Ctrl+C)
trap cleanup SIGINT


cleanup()
{
	echo "Signal received. Cleaning up..."
	pkill -P $$ nc
	exit 1
	# wait $!
}


# Connect to the IRC server
(
	sleep 0.5
	echo "PASS $PASSWORD"
	sleep 0.5
	echo "NICK $NICK"
	sleep 0.5
	echo "USER $USER 0 * :$REALNAME"
	sleep 0.5
	# echo "PRIVMSG $CHANNEL :$MESSAGE"
	echo "JOIN $CHANNEL"
	# sleep 1
	# sleep 1
	# echo "QUIT"
	# Connect to the IRC server
	while true; do
		if [ -t 0 ]; then
			# echo "$>" <&0
			read -t 1 -r input <&0
			if [ $? -eq 0 ]; then
					# Process the user input
				echo $input
				if [ "$input" == "QUIT" ]; then
					break
				fi
			fi
		fi
		if ! is_server_running 1>/dev/null ; then
			pkill -P $$ nc 1>&2
			break
		fi		
		# if [ "$input" == "QUIT" ]; then
		# 	break
		# fi
		# Break the loop if the input is "QUIT"

	done
	exec &1>-
) | nc $SERVER $PORT 2>/dev/null
# | while read server_response; do
#   # Print server response
#   echo "Server: $server_response"
  
#   # Check if the server is shutting down
#   if [[ "$server_response" == *"server is shutting down"* ]]; then
#     echo "Server is shutting down. Exiting..."
#     break
#   fi
# done