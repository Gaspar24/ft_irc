# Use an official Ubuntu as a parent image
FROM ubuntu:latest AS builder

# Set the working directory in the container
WORKDIR /usr/src/app

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential valgrind net-tools procps

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Build the ircserv executable
RUN make

# Use a smaller base image for the final stage
FROM ubuntu:latest

# Set the working directory in the container
WORKDIR /usr/src/app

# Install Valgrind in the final stage
RUN apt-get update && apt-get install -y valgrind make netcat-openbsd net-tools procps

# Copy the built executable from the builder stage
COPY --from=builder /usr/src/app/ircserv /usr/src/app/ircserv

# Copy the rest of the application files
COPY . .

# Command to run Valgrind
CMD ["valgrind", "./ircserv", "6667", "42"]