#!/bin/bash

SERVER="./ircserv"  # Path to your IRC server executable
PORT="6667"		 # Port your IRC server is listening on
MESSAGE="This is a test message to check the server buffer handling. This message is intentionally long to exceed 100 characters."

# Start the IRC server
# $SERVER $PORT pass
# echo SERVER_PID=$!
# sleep 2  # Give the server some time to start

# Function to start a client and send a message
start_client()
{
    {
        echo "NICK testuser$1"
        echo "USER testuser$1 0 * :Test User $1"
        echo "JOIN #testchannel"
        for j in $(seq 1 5); do
            echo "PRIVMSG #testchannel :$MESSAGE $j"
            sleep 1  # Delay between messages
        done
    } | nc localhost $PORT
}
# Start 30 clients
for i in $(seq 1 30); do
    start_client $i &
done
# Wait for all clients to finish
wait

# Stop the IRC server
# kill $SERVER_PID