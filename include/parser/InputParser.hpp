#ifndef INPUT_PARSER_HPP
# define INPUT_PARSER_HPP

# include <vector>
# include <string>
# include <sstream>
# include <iostream>

class InputParser
{
	public:
		static std::vector<std::string> parseInput(const std::string& args, char delimeter);
		static std::string trim(const std::string& str);
		static void printTokens(std::vector<std::string>& tokens);
};

#endif