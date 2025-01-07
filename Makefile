ifneq ($(shell test -d .settings && echo yes),)
include .settings/Makefile
endif
#------ TARGET ------#
NAME		:= ircserv
#------ WFLAGS ------#
D_FLAGS		= -Wall -Wextra -std=c++98 -Werror #-Wshadow #-pg #-Wno-unused-function -Wunused
INCLUDE_DIRS := $(shell find include -type d 2>/dev/null)
INC := $(addprefix -I,$(INCLUDE_DIRS))
#------ SRC FILES & DIRECTORIES ------#
SRC_DIR		:= src/
SRC			:= $(shell find $(SRC_DIR) -name '*.cpp' ! -name 'main.cpp')
MAIN		:= $(shell find $(SRC_DIR) -name 'main.cpp')
SRC			+= $(MAIN)
PROJECT_ROOT:= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/)
DIRS		:= $(abspath $(dir $(shell find $(PROJECT_ROOT) -name Makefile | sort)))
CURRENT		:= $(shell basename $$PWD)
DEBUG_PATH	:= debug
BUILD_DIR	:= .build/
#------ DEBUG FLAG ------#
D			= 0
#------ Sanitizer Flag ------#
S			= -1
#------ DEBUG UTILS ------#
CXX			:= c++
MAKEFLAGS	+= --no-print-directory #--jobs
VALGRIND	:= valgrind -s --leak-check=yes --show-leak-kinds=all -q
HELGRIND	:= valgrind -s --tool=helgrind
MAC_LEAKS	:= leaks -atExit --

#------ ADDING DEBUG FLAGS ------#
ifneq ($(D), 0)
CXX			= g++
DEBUG_DIR	:= $(NAME).dSYM
OBJS := $(addprefix $(BUILD_DIR), $(notdir $(SRC:%.cpp=%.o)))
#------ CODE FOR OBJECT FILES ------#

vpath %.cpp $(sort $(dir $(SRC))) #  list of directories that `make` should search when it's looking for prerequisites. 
DEPS		:= $(OBJS:.o=.d)
-include $(DEPS) # to include in compilation
D_FLAGS		+= -O0 -g3 #-Og optimization + good debuggin experience
# ASAN_OPTIONS=detect_leaks=1 # in case platform leaks detections is not enable
DEBUG_FLAG	:= $(D)
ifeq ($(DEBUG_FLAG), )
D_FLAGS		+= -DDEBUG=0
else
D_FLAGS		+= -DDEBUG=$(D)
endif
D_FLAGS		+= -Wconversion -Wsign-conversion -pedantic
D_FLAGS		+= -fno-omit-frame-pointer

else
OBJS		:= $(SRC)
endif
# MAKEFLAGS	+= -j4 #--debug #// -j for multinodes
ifeq ($(S), 1)
#------ INCLUDE SANATIZER FLAGS ------#
D_FLAGS		+= -pthread -fsanitize=thread,undefined,vptr -fno-optimize-sibling-calls
endif
ifeq ($(S), 2)
#------ INCLUDE SANATIZER FLAGS ------#
D_FLAGS		+= -fsanitize=address,undefined,vptr
endif
# To enable verbose output during compilation Make V=1
V ?= 0
ifeq ($(V),1)
VERBOSE = @
D_FLAGS		+= -ftime-report
else
VERBOSE =
endif

#-------------------- RULES ----------------------------#
all:$(NAME)

$(NAME): $(OBJS)
ifneq ($(D), 0)
	@printf "$(LF)\n$(P_BLUE)☑️  Successfully Created $(P_YELLOW)$(NAME)'s Object files ☑️$(FG_TEXT)\n"
endif
	@printf "\n"
	@printf "$(LF)⚙️ $(P_BLUE) Create $(P_GREEN)$@ ⚙️\n"
	@echo $(GREEN)
	@printf "$(CXX) $(D_FLAGS) $(INC) $(P_YELLOW) $^ $(P_GREEN) -o $@ $(FG_TEXT) \n\n";
	@$(CXX) $(D_FLAGS) $(INC) $^ -o $(NAME) -lpthread
	@printf "\n$(LF)✅ $(P_BLUE)Successfully Created $(P_GREEN)$@! ✅\n$(P_NC)"
	@echo $(CYAN) "$$CPP" $(E_NC)
	@echo "$$MANUAL" $(E_NC)

ifneq ($(D), 0)
$(OBJS): $(BUILD_DIR)%.o : %.cpp | $(BUILD_DIR)
	@$(CXX) $(D_FLAGS) $(INC) -MMD -MP -c $< -o $@
	@printf "$(LF)🚧 $(P_BLUE)Creating $(P_YELLOW)$@ $(P_BLUE)from $(P_YELLOW) $< $(FG_TEXT)\n"
	@printf "$(P_BLUE)$(CXX) $(D_FLAGS) $(INC) $(P_YELLOW) -MMD -MP -c $< $(P_BLUE) -o $@ $(FG_TEXT) \n\n";

$(BUILD_DIR):
	@mkdir -p $@
endif

.PHONY: clean fclean re val leaks test hexChat

clean:
	@echo;
	@if [ -d "$(BUILD_DIR)" ]; then	\
		rm -rf $(BUILD_DIR); 		\
		printf "$(LF)🧹 $(P_RED) Clean $(P_YELLOW)$(NAME)'s Object files$(P_NC)\n"; \
	fi
	@if [ -d "$(DEBUG_DIR)" ]; then	\
		rm -rf $(DEBUG_DIR); 		\
	fi
	@printf  "\n$(P_NC)"

fclean: clean
	@if [ -f $(NAME) ]; then	\
		printf "$(LF)🧹 $(P_RED) Clean $(P_GREEN) $(CURRENT)/$(NAME)\n";	\
		rm -rf $(NAME);														\
		echo $(WHITE) "$$TRASH" $(E_NC);									\
	else																	\
		printf "$(LF)🧹$(P_RED) Clean $(P_GREEN) $(CURRENT)\n";			\
	fi
	@printf "\n$(P_NC)"

re: fclean all

# Memmory leaks
# ATTENTION !!!!!!!!!!!!!!  USE WITH S=0 !
## do not use yet as it does not handle 
val: $(NAME)
	@echo $(RED) $(VALGRIND) ./$(NAME) $$port $$pass $(E_NC) "\n"
	@$(VALGRIND) ./$(NAME) $$port $$pass ; echo 
leaks: $(NAME)
	@echo $(RED)$(MAC_LEAKS) ./$(NAME) "$i" $(E_NC)  "\n"
	@$(MAC_LEAKS) ./$(NAME)
	@echo "\n"

#------------- TEST UTILS -----------------------------------#
#make check f=file -> check file syntax
check:
	@g++ $(D_FLAGS) $(INC) -fsyntax-only $(f); \
	ret=$$? ; \
	if [ $$ret = 0 ]; then \
		echo  $(f) [$(GREEN) OK $(E_NC)]; \
	fi

hel:$(NAME)
	@port="6667"; \
	pass="42";\
	if [ "$(S)" -lt 1 ]; then \
		echo $(RED) $(HELGRIND) ./$(NAME) $$port $$pass $(E_NC) "\n"; \
		$(HELGRIND) ./$(NAME) $$port $$pass; \
	fi;
#trap 'echo $(RED)"...Process interrupted" $(E_NC); exit 1' INT;
server: $(NAME)
	@port="6667"; \
	pass="42";\
	echo $(BOLD) $(CYAN) ./$(NAME) $$port $$pass $(E_NC) "\n"; \
	if [ "$(S)" = "0" ]; then \
		echo $(E_NC); $(MAKE) -C . hel; \
	else \
		if [ "$(S)" = "1" ]; then \
			echo $(E_NC); ./$(NAME) $$port $$pass; \
		else \
			echo $(E_NC); ./$(NAME) $$port $$pass; \
		fi; \
	fi; \
	ret=$$?; \
	if [ $$ret -eq 0 ]; then \
		echo $(GREEN)"server finished"  $(E_NC); \
	else \
		echo $(RED)"...server terminated"  $(E_NC); \
	fi;

client: $(NAME)
	@address="localhost"; \
	port="6667";\
	echo $(BOLD) $(CYAN) nc -C $$address $$port $(E_NC) "\n"; \
	if [ "$(S)" = "0" ]; then \
		echo $(E_NC); $(MAKE) -C . val; \
	else \
		if [ -z "$(i)" ]; then \
			echo $(E_NC); nc -C $$address $$port ; \
		else \
			echo $(E_NC); ./$(NAME) $(i); \
		fi; \
	fi
test:$(NAME)
	./testScripts/runClients.sh
dclient:$(NAME)
	@chmod +x testScripts/defaultClient.sh
	@trap 'echo $(RED)"...client process interrupted" $(E_NC);exit 0' INT;\
	./testScripts/defaultClient.sh ; \
	ret=$$?; \
	if [ $$ret -eq 0 ]; then \
		echo $(GREEN)"dClient finished"  $(E_NC); \
		exit 0; \
	else \
		echo $(RED)"...dClient terminated. Server not found"  $(E_NC); \
	fi;
delUsers:
	@pgrep -x "hexchat" > /dev/null; \
	ret=$$?; \
	if [ $$ret -eq 0 ]; then \
		echo $(GREEN)"HexChat is running"  $(E_NC); \
	else \
		echo $(RED)"...Deleting usersFile"  $(E_NC); \
		rm -f testScripts/users_created.txt; \
	fi;
delHex:
	@path="$(HOME)/.var/app/io.github.Hexchat/config/hexchat" ;\
	rm -rf $$path/addons/* $$path/scrollback/irc/#* ; \
	ret=$$?; \
	if [ $$ret -eq 0 ]; then \
		echo $(GREEN)"HexChat Addons deleted"  $(E_NC); \
	else \
		echo $(RED)"...Error removing addons"  $(E_NC); \
	fi;
thex: delHex addon
	@flatpak run io.github.Hexchat --command="set text_font Ubuntu Mono 9 " --command="py load ~/.var/app/io.github.Hexchat/config/hexchat/addons/handshake.py" --command="server irc"
hex: $(NAME) delUsers addon
	@trap 'clear; echo ;echo $(RED)"...Process interrupted" $(E_NC); exit 1' INT; \
	python3 testScripts/openHexchat.py; echo $(GREEN)"Process Ended" $(E_NC);
addon:
	@chmod +r $(PWD)/testScripts/testHexChat.py
	@mkdir -p $(HOME)/.var/app/io.github.Hexchat/config/hexchat/addons/
#	@cp $(PWD)/testScripts/testHexChat.py $(HOME)/.var/app/io.github.Hexchat/config/hexchat/addons
	@chmod +r $(PWD)/testScripts/handshake.py
	@cp $(PWD)/testScripts/handshake.py $(HOME)/.var/app/io.github.Hexchat/config/hexchat/addons
	@echo $(GREEN)...Addon added to  HexChat path $(E_NC)
# flatpak run io.github.Hexchat --command="py load ~/.var/app/io.github.Hexchat/config/hexchat/addons/testHexChat.py"

# #-------------------- PROCESS UTILS ----------------------------#
killPD:
	@echo "Closing all processes related to $(NAME)..."
	@pkill -f $(NAME)
	@echo "All related processes have been closed."

closeFD:
	@echo "Finding process ID(s) for process name: $(NAME)"
	@pids=$$(pgrep $(NAME)); \
	if [ -z "$$pids" ]; then \
		echo "No process found with name: $(NAME)"; \
	else \
		for pid in $$pids; do \
			echo "Closing file descriptors for process ID: $$pid"; \
			for fd in /proc/$$pid/fd/*; do \
				if [ -e $$fd ]; then \
					exec 3>&-; \
				fi; \
			done; \
		done; \
	fi
checkOpen:
	@echo $(WHITE)"Checking open files for process name: $(NAME)"$(E_NC)
	@pids=$$(pgrep $(NAME)); \
	if [ -z "$$pids" ]; then \
		echo $(RED)"No process found with name: $(NAME)"$(E_NC); \
	else \
		for pid in $$pids; do \
			echo "Open files for process ID: $$pid"; \
			watch -n 2 "lsof -p $$pid 2>/dev/null | awk 'NR==1 || \$$4 ~ /u\$$/ {print \$$1, \$$2, \$$4, \$$5, \$$9, \$$10, \$$11}'"; \
		done; \
	fi
freePort:
	@port="6667";\
	pids=$$(lsof -t -i :$$port); \
	if [ -z "$$pids" ]; then \
		echo $(WHITE)"Port $$port is already free."$(E_NC); \
	else \
		echo $(YELLOW)"Freeing port $$port..."$(E_NC); \
		echo $(YELLOW)"Terminating processes using port $$port: $$pids" $(E_NC); \
		kill -9 $$pids; \
		echo $(GREEN)"Port $$port has been freed." $(E_NC); \
	fi
# #-------------------- UTILS ----------------------------#
info:
	@echo GIT_REPO:  $(CYAN) $(GIT_REPO) $(E_NC)
	@echo PROJECT_ROOT: $(CYAN) $(PROJECT_ROOT) $(E_NC)
	@echo DIRS: $(BOLD) $(DIRS) "\n" $(E_NC)
	@echo CURRENT: $(GREEN) $(CURRENT) $(E_NC)
	@echo SRC: $(YELLOW) $(SRC) $(E_NC)
	@echo OBJS: $(GRAY) $(OBJS) $(E_NC)
	@echo OBJS: $(MAG) $(INC) $(E_NC)
	@echo $(BLUE)"\nD = $(D)" $(E_NC)
	@echo $(BLUE)"S = $(S)" $(E_NC)
	@echo $(BLUE)"V = $(V)" $(E_NC)
	@echo $(BLUE)"DEPENDENCIES =" $(DEPENDENCIES) $(E_NC)


cpp: # Usage: make cpp h=filename
	@if [ ! -d "include" ]; then \
		mkdir include; \
	fi
	@if [ ! -z "$(h)" ] && [ ! -f "include/$(h).hpp" ]; then \
		UPPER_H=$$(echo $(h) | tr a-z A-Z)_HPP; \
		HEADER_PATH="include/$(h).hpp"; \
		printf "$${HEADER_CONTENT}" | sed -e 's/{{UPPER_H}}/'"$$UPPER_H"'/' \
		-e 's/{{CLASS_NAME}}/$(h)/g' > $$HEADER_PATH; \
		echo "$$HEADER_PATH was created"; \
	fi
	@if [ ! -d "src" ]; then \
		mkdir src; \
	fi
	@for file in include/*.hpp; do \
		cpp_file="src/$$(basename "$${file%.*}").cpp"; \
		if [ ! -f "$$cpp_file" ]; then \
			class_name="$$(basename "$${file%.*}")"; \
			echo "$${CPP_CONTENT}" | sed -e "s/{{CLASS_NAME}}/$$class_name/g" > "$$cpp_file"; \
			echo "$$cpp_file was created"; \
		fi; \
	done
	@echo "Recently Created/Modified files:"; find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -mmin -5 | sort -r
content:
	@find . -type f \( -name "*.cpp" -o -name "*.hpp" \) | sort

# Usage: make main
main:
	@if [ ! -f "src/main.cpp" ]; then \
		printf "$${MAIN_CONTENT}" > src/main.cpp; \
		echo "src/main.cpp was created"; \
	fi

#--------------------COLORS----------------------------#
# For print
CL_BOLD  = \e[1m
RAN	 	 = \033[48;5;237m\033[38;5;255m
D_PURPLE = \033[1;38;2;189;147;249m
D_WHITE  = \033[1;37m
NC	  	 = \033[m
P_RED	 = \e[1;91m
P_GREEN  = \e[1;32m
P_BLUE   = \e[0;36m
P_YELLOW = \e[1;33m
P_CCYN   = \e[0;1;36m
P_NC	 = \e[0m
LF	   = \e[1K\r$(P_NC)
FG_TEXT  = $(P_NC)\e[38;2;189;147;249m
# For bash echo
CLEAR  = "\033c"
BOLD   = "\033[1m"
CROSS  = "\033[8m"
E_NC   = "\033[m"
RED	= "\033[1;31m"
GREEN  = "\033[1;32m"
YELLOW = "\033[1;33m"
BLUE   = "\033[1;34m"
WHITE  = "\033[1;37m"
MAG	= "\033[1;35m"
CYAN   = "\033[0;1;36m"
GRAY   = "\033[1;90m"
PURPLE = "\033[1;38;2;189;147;249m"

define CPP

		⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄
		⣠⣾⣿⡟⠛⢻⠛⠛⠛⠛⠛⢿⣿⣿⠟⠛⠛⠛⣿⣿⣷⣄
		⣿⣿⣿⡇⠀⢸⠀⠀⣿⣿⡇⠀⣿⠁⠀⣠⣤⣤⣿⣿⣿⣿
		⣿⣿⣿⡇⠀⢸⠀⠀⠿⠿⠃⣠⣿⠀⠀⣿⣿⣿⣿⣿⣿⣿
		⣿⣿⣿⡇⠀⢸⠀⠀⣀⣀⠀⠙⣿⠀⠀⣿⣿⣿⣿⣿⣿⣿
		⣿⣿⣿⣇⣀⣸⣀⣀⣿⣿⣀⣀⣿⣦⡀⣀⣀⣀⣿⣿⣿⣿
		⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⢿⣿⣿⣿⣿⣿⣿
		⣿⣿⣿⣿⣿⣿⡿⠿⠛⠿⡿⠉⠀⠀⠀⠀⠈⠹⣿⣿⣿⣿
		⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⢇⠀⠛⠘⠃⠛⠀⢠⣿⣿⣿⣿
		⣿⣿⣿⣿⣧⡀⠛⠘⠃⠛⠀⢑⣤⣄⣀⣤⡀⣿⣿⣿⣿⣿
		⣿⣿⣿⣿⣿⡗⢀⣀⣀⣀⣤⣾⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿
		⠙⢿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋
		⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋

endef
export CPP
define TRASH

			⠀⠀⠀⠀⠀⠀⢀⣠⣤⣤⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀
			⠀⠀⠀⠀⣰⣾⠋⠙⠛⣿⡟⠛⣿⣿⣿⣷⣆⠀⠀⠀⠀
			⠀⠀⢠⣾⣿⣿⣷⣶⣤⣀⡀⠐⠛⠿⢿⣿⣿⣷⡄⠀⠀
			⠀⢠⣿⣿⣿⡿⠿⠿⠿⠿⠿⠿⠶⠦⠤⢠⣿⣿⣿⡄⠀
			⠀⣾⣿⣿⣿⣿⠀⣤⡀⠀⣤⠀⠀⣤⠀⢸⣿⣿⣿⣷⠀
			⠀⣿⣿⣿⣿⣿⠀⢿⡇⠀⣿⠀⢠⣿⠀⣿⣿⣿⣿⣿⠀
			⠀⢿⣿⣿⣿⣿⡄⢸⡇⠀⣿⠀⢸⡏⠀⣿⣿⣿⣿⡿⠀
			⠀⠘⣿⣿⣿⣿⡇⢸⡇⠀⣿⠀⢸⡇⢠⣿⣿⣿⣿⠃⠀
			⠀⠀⠘⢿⣿⣿⡇⢸⣧⠀⣿⠀⣼⡇⢸⣿⣿⡿⠁⠀⠀
			⠀⠀⠀⠀⠻⢿⣷⡘⠛⠀⠛⠀⠸⢃⣼⡿⠟⠀⠀⠀⠀
			⠀⠀⠀⠀⠀⠀⠈⠙⠛⠛⠛⠛⠛⠋⠁⠀⠀⠀⠀⠀⠀
endef
export TRASH

define MANUAL
$(D_PURPLE)
FLAGS:
	D -> debug information	 (default: 0)
	S -> sanatizer information (default: -1)

Example:
$(D_WHITE)[run server]
$(D_PURPLE)$> make server
$(D_WHITE)[run client]
$(D_PURPLE)$> make client
$(D_WHITE)[test + DEBUG]
$(D_PURPLE)$> make D=1 server || client


endef
export MANUAL

define HEADER_CONTENT
#ifndef {{UPPER_H}}
# define {{UPPER_H}}

# include <iostream>
# include <sstream>
# include <string>
# include <typeinfo>
# include <Utils.hpp>

# ifndef DEBUG
#  define DEBUG 0
# endif

class {{CLASS_NAME}}
{
	private:

	protected:
		
	public:
		{{CLASS_NAME}}();
		{{CLASS_NAME}}& operator=({{CLASS_NAME}} const& rhs);
		{{CLASS_NAME}}({{CLASS_NAME}} const& rhs);
		virtual ~{{CLASS_NAME}}();
};

// std::ostream& operator << (std::ostream& os, {{CLASS_NAME}}& rhs);

#endif // {{UPPER_H}}
endef
export HEADER_CONTENT
define MAIN_CONTENT
#include <iostream>
$(shell for file in include/*.hpp; do \
	printf '#include "'$$(basename $$file)'"\\n';\
done)
int main(void)
{
	return (0);
}
endef
export MAIN_CONTENT

define CPP_CONTENT
#include "{{CLASS_NAME}}.hpp"

{{CLASS_NAME}}::{{CLASS_NAME}}()
{
}

{{CLASS_NAME}}& {{CLASS_NAME}}::operator=({{CLASS_NAME}} const& rhs)
{

}

{{CLASS_NAME}}::{{CLASS_NAME}}({{CLASS_NAME}} const& rhs)
{

}

{{CLASS_NAME}}::~{{CLASS_NAME}}(void)
{
}

std::ostream& operator << (std::ostream& os, {{CLASS_NAME}}& rhs)
{
	(void)rhs;
	return (os);
}
endef
export CPP_CONTENT

#find . -type f -name "*.*pp" -print | sort -r |  xargs cat | pygmentize -g -O style=material
