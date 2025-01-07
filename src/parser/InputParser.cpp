# include "InputParser.hpp"
# include "Utils.hpp"

std::string InputParser::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	if (first == std::string::npos || last == std::string::npos) {
		return "";
	} else {
		return str.substr(first, last - first + 1);
	}
}

std::vector<std::string> InputParser::parseInput(const std::string& args, char delimeter) {
	std::vector<std::string> tokens;
	std::string trimmedArgs = InputParser::trim(args);
	printAsciiDecimal(trimmedArgs);
	std::stringstream stream(trimmedArgs);
	std::string token;

	// Check for semicolon in args
	size_t colonPos = trimmedArgs.find(':');
	if (colonPos != std::string::npos) {
		std::string beforeColon = trimmedArgs.substr(0, colonPos);
		std::string afterColon = trimmedArgs.substr(colonPos + 1);

		std::stringstream beforeStream(beforeColon);
		while(std::getline(beforeStream, token, delimeter)) {
			token = InputParser::trim(token);
			tokens.push_back(token);
		}
		tokens.push_back(afterColon);
	} else {
		// No semicolon found, split the entire string
		while (std::getline(stream, token, delimeter)) {
			token = InputParser::trim(token);
			tokens.push_back(token);
		}
	}

	return tokens;
}

void InputParser::printTokens(std::vector<std::string>& tokens) {
	size_t index = 0;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++, index++) {
		*it = trim(*it);
		Utils::safePrint("Target [" + toStr(index) + "]: " + toStr(*it));
	}
}