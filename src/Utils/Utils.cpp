#include "Utils.hpp"

/**
 * Prints the specified number of new lines.
 *
 * @param newLines The number of new lines to print.
 */
void nl(size_t newLines)
{
	for (size_t i=1 ; i <= newLines; ++i)
		std::cout << std::endl;
}

/**
 * @brief Prints the given string to the standard error stream if the DEBUG flag is enabled.
 *
 * @param eColor The color of the string.
 * @param str The string to be printed.
 */
void debug(int eColor, std::string str)
{
	if (DEBUG)
		std::cerr << getColorStr(eColor, str);
}

/**
 * @brief Prints a string followed by a newline.
 *
 * This function takes a string as input and prints it to the standard output
 * followed by a newline. It uses the std::cout object from the iostream
 * library to print the string.
 *
 * @param str The string to print.
 * std::string string1 = "Hello";
 * std::string string2 = "World";
 * printnl(std::cout, string1);
 * printnl(std::cout, string2);
 */
void out(std::ostream& os)
{
	if (DEBUG > 0)
		os << std::endl;
	else
	{
		os.flush();
		os.clear();
		os << std::endl;
	}
}


/**
 * @brief Formats a string with color for console output.
 *
 * This function takes a message string, a color code, and a boolean error flag.
 * It returns the message string formatted with the specified color. If the error
 * flag is true, it prepends "ERROR:" to the message and sets the color to red,
 * regardless of the specified color code.
 *
 * @param msg The message string to be formatted.
 * @param color The color code for the message. This should be a value from the
 *			  t_color enum.
 * @param err A boolean flag indicating whether the message is an error message.
 * @return The formatted message string.
 */
std::string	getColorFmt(int eColor)
{
	std::ostringstream strColor;
	std::string	fmt;

	fmt = C_FMT;
	if (eColor < 0)
		eColor = DEFAULT;
	if (eColor > FGRAY)
		fmt += "1;";
	else
		fmt += "0;";
	strColor << fmt << eColor << "m";
	return (strColor.str());
}

t_color getRandomAnsiiColor()
{
	// Array of all possible colors in the eColor enum
	t_color colors[] = {
		FLBLACK, FLRED, FLGREEN, FLYELLOW, FLBLUE, FLMAGENTA, FLCYAN, FGRAY, FRED, FGREEN, FYELLOW, FBLUE, FMAGENTA, FCYAN
	};

	// Get the number of colors in the array
	int num_colors = sizeof(colors) / sizeof(colors[0]);

	// Seed the random number generator
	initSeed();
	// Select a random index
	int random_index = rand() % num_colors;

	// Return the random color
	return colors[random_index];
}
/**
 * Generates an error message with optional formatting.
 *
 * @param str The error message to be displayed.
 * @param bold A boolean value indicating whether the error message should be
 * displayed in bold.
 * @return The formatted error message.
 */
std::string error(std::string str, bool bold)
{
	std::ostringstream strColor;
	std::string	fmt;

	fmt = C_FMT;
	if (bold)
		fmt += "1;";
	else 
		fmt += "0;";
	strColor << fmt << FRED << "m"
	<< "ERROR: " << str << C_END;
	return (strColor.str());
}

/**
 * Concatenates the given color string with the input string and the default color string.
 * 
 * @param eColor The color string to be concatenated.
 * @param str The input string to be concatenated.
 * @return The concatenated string with the color string and the default color string.
 */
std::string	getColorStr(std::string const& eColor, std::string const& str)
{
	return (eColor + str + std::string(C_END));
}

/**
 * Returns a string with the specified color formatting applied.
 *
 * @param eColor The color code to apply to the string.
 * @param str The input string.
 * @return The formatted string with the specified color.
 */
std::string	getColorStr(int eColor, std::string const& str)
{
	std::ostringstream os;
	os << getColorFmt(eColor) << str << C_END;
	return (os.str());
}

std::string	getColorStr(int eColor, lsi num)
{
	std::ostringstream os;
	os << getColorFmt(eColor) << num << C_END;
	return (os.str());
}

/**
 * @brief Generates an RGB color string for terminal output.
 *
 * Constructs a string that represents an RGB color using ANSI escape codes.
 * This string can be used to set text color in terminal outputs. The function
 * takes three integers representing the red, green, and blue components of the
 * color, each in the range [0, 255].
 *
 * @param red The red component of the color, [0, 255].
 * @param green The green component of the color, [0, 255].
 * @param blue The blue component of the color, [0, 255].
 * @return std::string The ANSI escape code string for the RGB color.
 */
static std::string	rColorRGB(int red, int green, int blue)
{
	std::ostringstream ss;

	ss << FLRGB << ";2;";
	ss << red << ";" << green << ";" << blue << "m";
	return (ss.str());
}

/**
 * Generates a random integer between the given minimum and maximum values.
 *
 * @param min The minimum value for the random number (inclusive).
 * @param max The maximum value for the random number (inclusive).
 * @return A random integer between the minimum and maximum values.
 */
int ft_rand(int min, int max)
{
	static bool seeded = false;
	if (!seeded)
	{
		srand(static_cast<unsigned int>(time(0)));
		seeded = true;
	}
	if (min < 0 || min > 256|| max < 0 || max > 256)
		return (255);
	return (rand() % (max - min + 1) + min);
}

/**
 * @brief Generate a colored string with optional bold formatting.
 * 
 * @param bold Whether to apply bold formatting.
 * @return std::string The colored string.
 * strColor << fmt
	<< rColorRGB(ft_rand(80, 200), ft_rand(80, 200), ft_rand(80, 200));
 */
std::string getRandomColorFmt(bool bold, bool standard)
{
	std::ostringstream strColor;

	strColor << C_FMT;
	if (bold)
		strColor << + "1;";
	else
		strColor << + "0;";
	if (standard)
		strColor << getRandomAnsiiColor() << "m";
	else
		strColor << rColorRGB(ft_rand(60, 255), ft_rand(60, 255), ft_rand(60, 255));
	return (strColor.str());
}

/**
 * @brief Generates a color shade based on the given color and pattern.
 * 
 * This function takes an integer value representing the color and pattern, and returns a string
 * representing the generated color shade. The color shade is generated by appending the color and
 * pattern values to a string stream and returning the resulting string.
 * 
 * @param eColor The integer value representing the color.
 * @param pattern The integer value representing the pattern.
 * @return A string representing the generated color shade.
 */
std::string	getColorShade(int eColor, int pattern)
{
	std::ostringstream strColor;

	strColor << C_FMT << eColor;
	if (toStr<int>(pattern).find_first_of("68") == std::string::npos)
		return (strColor.str() + "m");
	strColor << ";" << pattern << "m";
	return (strColor.str());
}

/**
 * @brief Returns a string representing the color shade based on the given color code.
 * 
 * @param eColor The color code.
 * @return std::string The color shade string.
 */
std::string	getColorShade(int eColor)
{
	std::ostringstream strColor;
	static int pattern = -1;
	pattern++;
	if (toStr<int>(pattern).find_first_of("68") != std::string::npos)
		pattern++;
	strColor << C_FMT << eColor;
	if (pattern != 0)
		strColor << ";" << pattern;
	strColor << "m";
	if (pattern > 9)
		pattern = 0;
	return (strColor.str());
}

/**
 * @brief Sets the color name based on the provided color code.
 * 
 * @param color The color code.
 * @return std::string The color name.
 */
std::string setObjColor(int const& color)
{
	if (DEBUG == 0)
	{
		if ((color > FWHITE && color < BGRAY ) || color > 107)
			return(getRandomColorFmt(1, 0));
		return (getColorFmt(color));
	}
	else
		return (getRandomColorFmt(1, 0));
}

/**
 * @brief Checks if all characters in a string satisfy a given condition.
 *
 * @param str The input string to be checked.
 * @param check_type A function pointer to the condition that each character
 * must satisfy.
 * @return true if all characters satisfy the condition, false otherwise.
 */
bool	checkInput(const std::string& str, int (*check_type)(int))
{
	std::string::const_iterator it;
	
	it = str.begin();
	while (it != str.end())
	{
		if (!check_type(*it) && !std::isspace(*it))
			return false;
		it++;
	}
	return true;
}

/**
 * @brief Checks if a string contains only spaces.
 *
 * This function takes a string as input and iterates through each character.
 * If it finds a character that is not a space, it returns false. If it
 * doesn't find any non-space characters, it returns true.
 *
 * @param str The string to check.
 * @return true if the string contains only spaces, false otherwise.
 */
bool	isOnlySpaces(const std::string& str)
{
	std::string::const_iterator it;
	
	for (it = str.begin(); it != str.end(); it++)
	{
		if (*it != ' ')
			return false;
	}
	return (true);
}

std::string toUpperCase(std::string const& str)
{
	std::string upperCase(str);

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (std::islower(str[i]))
			upperCase.replace(i, 1, 1, static_cast<char>(std::toupper(str[i])));
	}
	return (upperCase);
}

/**
 * @brief Finds the maximum string length in an array.
 *
 * This function takes an array size and a string array as input. It iterates
 * through the array and finds the length of the longest string. It returns
 * the length of the longest string.
 *
 * @param arraySize The size of the string array.
 * @param arrayData The string array to search.
 * @return The length of the longest string in the array.
 */
size_t	maxStringLength(int arraySize, std::string* arrayData)
{
	int			i;
	size_t		len;

	i = 0;
	len = 0;
	while (i < arraySize)
	{
		if (len < arrayData[i].length())
			len = arrayData[i].length();
		i++;
	}
	return (len);
}

/**
 * @brief Centers a string within a specified width.
 *
 * This function takes a string and a target width, then pads the string with
 * spaces on both sides such that it appears centered if printed in a field of
 * the given width. If the required padding is odd, the extra space is added to
 * the left side of the string.
 *
 * @param s The string to center.
 * @param width The total width of the field in which to center the string.
 * @return std::string The centered string, padded with spaces.
 */
std::string	center(const std::string& s, std::string::size_type width)
{
	std::string::size_type len = s.length();
	if (len >= width) {
		return s;
	}
	size_t padding = (width - len) / 2;
	return std::string(padding, ' ') + s + std::string(width - len - padding, ' ');
	// if (width <= s.size())
	// 	return s;
	// std::string::size_type padding = width - s.size();
	// std::string::size_type left = padding / 2 + padding % 2;
	// std::string::size_type right = padding / 2;
	// return (std::string(left, ' ') + s + std::string(right, ' '));
}


std::string errorFmt(const std::string& s, int width)
{
	std::ostringstream oss;
	oss << std::left << std::setw(width) 
	<< std::setfill(' ') << s ;
	return (error(oss.str() + "=> ", 0));
}
/**
 * @brief Extracts and returns the class name from a mangled string.
 *
 * This function takes a mangled class name as input and returns a cleaner
 * version of the class name by removing any leading numeric characters. If the
 * input string does not contain any non-numeric characters, an empty string is
 * returned.
 *
 * @param str The mangled class name string.
 * @return std::string The cleaned class name without leading numeric
 * characters.
 */
std::string className(const std::string& str)
{
	size_t pos = str.find_first_not_of("0123456789");
	if (pos != std::string::npos)
		return str.substr(pos);
	else
		return "";
}

/**
 * @brief Demangles a mangled C++ name.
 * 
 * This function takes a mangled C++ name as input and attempts to demangle it
 * using the `abi::__cxa_demangle` function. If the demangling is successful,
 * it returns the demangled name as a `std::string`. If the demangling fails or
 * the input is not a mangled name, it returns the original mangled name.
 * 
 * @param mangled_name The mangled C++ name to demangle.
 * @return std::string The demangled C++ name, or the original mangled name if demangling fails.
 */
std::string demangle(const char* mangled_name)
{
	int status = 0;
	size_t length = 1024;
	char buffer[1024];

	char* demangled = abi::__cxa_demangle(mangled_name, buffer, &length, &status);

	if (status != 0 || !demangled)
		return (std::string(mangled_name));
	std::string result(buffer);
	return (result);
}

/**
 * Concatenates the given class name and function name with "::" separator.
 *
 * @param c The class name.
 * @param f The function name.
 * @return The concatenated string.
 */
std::string classFunctionStr(std::string const& c, std::string const& f)
{
	return (c + "::" + f + " ");
}
/**
 * Generates a random string of length 10.
 * 
 * @return The randomly generated string.
 */
std::string getRandString()
{
	static bool seeded = false;
	std::string randomStr= "";
	size_t strLen = 10;

	if (!seeded)
	{
		srand(static_cast<unsigned int>(time(0)));
		seeded = true;
	}
	for(size_t i = 0; i < strLen; i++)
	{
		char random_char = 'a' + static_cast<char>(rand() % 26); // random character between 'a' and 'z'
		randomStr += random_char;
	}
	return (randomStr);
}

/**
 * Prints a title surrounded by equal signs.
 * 
 * @param title The title to be printed.
 */
void printTitle(std::string title, int n)
{
	int size = n;

	if (DEBUG == 0)
		return ;
	std::string toPrint = " " + title + " "; 

	if (size <= 0 || size > 60)
		size = 60;
	int len = static_cast<int>(toPrint.size());
	int padding = (size - len) / 2;

	if (len % 2 != 0 && size % 2 == 0)
		padding++;

	std::cout << std::setfill('=') << std::setw(padding) << "";
	std::cout << toPrint;
	std::cout << std::setfill('=') << std::setw(size - len - padding)
	<< "" << std::endl;
}

size_t getRandomNum(size_t num)
{
	static bool seeded = false;
	if (!seeded)
	{
		srand(static_cast<unsigned int>(time(0)));
		seeded = true;
	}
	return (static_cast<size_t>(rand()) % num);
}

/**
 * Counts the number of newline characters in a given string.
 *
 * @param str The string to count newlines in.
 * @return The number of newline characters in the string.
 */
size_t countNewlines(const std::string& str)
{
	size_t count = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == '\n')
			++count;
	}
	return (count);
}

void printTitle(std::string title, int n, char c)
{
	int size = n;
	std::string toPrint;

	if (!title.empty())
		toPrint = " " + title + " "; 

	if (size <= 0 || size > 60)
		size = 60;
	int len = static_cast<int>(toPrint.size());
	int padding = (size - len) / 2;

	if (len % 2 != 0 && size % 2 == 0)
		padding++;

	std::cout << std::setfill(c) << std::setw(padding) << "";
	std::cout << toPrint;
	std::cout << std::setfill(c) << std::setw(size - len - padding)
	<< "" << std::endl;
}

void initSeed()
{
	static bool seeded = false;
	
	if (!seeded)
	{
		srand(static_cast<unsigned int>(time(0)));
		seeded = true;
	}
}

void osPrint(std::ostream& os, int const& val)
{
	os << val << " ";
}

void printAsciiDecimal(const std::string& str){
	if (DEBUG < 2)
		return ;
	std::ostringstream os;

	for (size_t i = 0; i < strlen(str.c_str()); ++i)
	{
		os << static_cast<int>(str[i]) << ",";
	}
	os << std::endl;
	Utils::safePrint(os.str());
}

std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\n\r");
	size_t last = str.find_last_not_of(" \t\n\r");
	if (first == std::string::npos || last == std::string::npos) {
		return "";
	} else {
		return str.substr(first, last - first + 1);
	}
}

bool isNumber(const std::string& str) {
	
	if (str.empty()) {
		return false;
	}
	unsigned long int i = 0;
	if (str[0] == '-' || str[0] == '+') {
		i++;
		if (i == str.size()) {
			return false;
		}
	}

	for (; i < str.size(); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

long int modAtoi(std::string nb){

	long int result = 0;
	int flag = 1;
	unsigned long int i = 0;

	if (nb[0] == '-') {
		flag = -1;
		i++;
	} else if (nb[0] == '+') {
		i++;
	}

	for (; i < nb.size(); i++) {
		if (nb[i] >= '0' && nb[i] <= '9') {
			result = result * 10 + (nb[i] - '0');
		} else {
			return -1;
		}
	}

	return result * flag;
}


/**
 * @brief Global mutex for thread-safe console output operations
 * 
 * This static mutex protects access to std::cout across multiple threads,
 * preventing race conditions when writing to the console. It is initialized
 * using PTHREAD_MUTEX_INITIALIZER for C++98 compliance.
 *
 * Usage:
 * - Lock before using std::cout operations
 * - Unlock after console output is complete
 * - Used primarily by Utils::safePrint() for thread-safe console output
 *
 * @note This mutex must be destroyed at program exit via Utils::cleanupMutex()
 */
pthread_mutex_t Utils::coutMutex = PTHREAD_MUTEX_INITIALIZER;

void Utils::cleanupMutex() {
	pthread_mutex_destroy(&coutMutex);
}

/**
 * @brief Helper function to register mutex cleanup
 */
static int registerMutexCleanup() {
	std::atexit(Utils::cleanupMutex);
	return 0;
}

/**
 * @brief Static initialization to register mutex cleanup handler
 */
static int registerCleanup = registerMutexCleanup();

void Utils::safePrint(const std::string& message) {
	if (!DEBUG)
		return ;
	pthread_mutex_lock(&coutMutex);
	std::cout << message << std::endl;
	pthread_mutex_unlock(&coutMutex);
}

std::string Utils::truncateString(const std::string& string) {
	if (string.length() <= USERLEN) {
		return string;
	}
	return (string.substr(0, USERLEN - 3)) + "...";
}
