#ifndef UTILS_H
# define UTILS_H

# include <iostream>
# include <string>
# include <sstream>
# include <cstdlib>
# include <iomanip>
# include <cxxabi.h>
# include <cstring>
# include <cstdio>
# include "NumericMessages.hpp"
# ifndef DEBUG
#  define DEBUG 0
# endif

# define C_FMT "\033["
# define C_FMT256 "\033[0;38;"
# define C_FMT256B "\033[1;38;"
# define FLPURPLE C_FMT256 "2;189;147;249m"
# define FPURPLE C_FMT256B "2;189;147;249m"
# define FLORANGE C_FMT256 "5;208m"
# define FORANGE C_FMT256B "5;208m"
# define C_END "\033[0m"


/*
	FL is light foreground color
	B is background color
	F is foreground color
*/
typedef enum eColor
{
	DEFAULT			= 0,
	FLBLACK			= 30,
	FLRED			= 31,
	FLGREEN			= 32,
	FLYELLOW		= 33,
	FLBLUE			= 34,
	FLMAGENTA		= 35,
	FLCYAN			= 36,
	FLWHITE			= 37,
	FLRGB			= 38, //\033[38;2;r;g;bm (rgb:from 0 to 255).
	FDEFAULT		= 39,
	BBLACK			= 40,
	BRED			= 41,
	BGREEN			= 42,
	BYELLOW			= 43,
	BBLUE			= 44,
	BMAGENTA		= 45,
	BCYAN			= 46,
	BWHITE			= 47,
	BRGB			= 48, //\033[48;2;r;g;bm (rgb:from 0 to 255).
	BDEFAULT		= 49,
	FGRAY			= 90,
	FRED			= 91,
	FGREEN			= 92,
	FYELLOW			= 93,
	FBLUE			= 94,
	FMAGENTA		= 95,
	FCYAN			= 96,
	FWHITE			= 97,
	BGRAY			= 100
}	t_color;

typedef long double lsi;

std::string error(std::string str, bool bold);
void nl(size_t newLines);
void		out(std::ostream& os);
void		debug(int eColor, std::string str);
std::string	getColorFmt(int eColor);
// std::string	getColorStr(t_Name const& name);
std::string	getColorStr(std::string const& eColor, std::string const& str);
std::string	getColorStr(int eColor, std::string const& str);
std::string	getColorStr(int eColor, lsi num);
int			ft_rand(int min, int max);
t_color 	getRandomAnsiiColor(bool bold);
std::string	getRandomColorFmt(bool bold, bool standard);
std::string	getColorShade(int eColor, int pattern);
std::string	getColorShade(int eColor);
std::string	setObjColor(int const& color);
bool		checkInput(const std::string& str, int (*check_type)(int));
bool		isOnlySpaces(const std::string& str);
std::string toUpperCase(std::string const& str);
size_t		maxStringLength(int fieldSize, std::string* arrayData);
std::string	center(const std::string& s, std::string::size_type width);
std::string errorFmt(const std::string& s, int width = 22);
std::string	className(const std::string& str);
std::string	classFunctionStr(std::string const& c, std::string const& f);
std::string	demangle(const char* mangled_name);
void		printTitle(std::string title, int n);
size_t		countNewlines(const std::string& str);
void		printTitle(std::string title, int n, char c);
void		initSeed();
void		osPrint(std::ostream& os, int const& val);
void		printAsciiDecimal(const std::string& str);
std::string trim(const std::string& str);
bool 		isNumber(const std::string& str);
long int 	modAtoi(std::string nb);

class Utils {
	public:
		static void safePrint(const std::string& message);
		static void cleanupMutex();
		static std::string truncateString(const std::string& string);

		static pthread_mutex_t coutMutex;
};

#include <Utils.tpp>
#endif

/**
 * @NOTES:
 * When you define template functions in a header file, they are
   included in multiple translation units (source files). Without the
   `inline` keyword, the compiler generates multiple definitions of
   the same function, leading to linker errors. The `inline` keyword
   tells the compiler that the function can have multiple definitions
   across different translation units, but they should be treated as a
   single definition.

Here's a step-by-step explanation:

1. **Template Functions in Header Files**: Template functions are
   typically defined in header files because their definitions need to
   be available wherever they are instantiated. This means the
   compiler needs to see the full definition of the template function
   to generate the appropriate code for each instantiation.

2. **Multiple Inclusion**: When a header file containing template
   function definitions is included in multiple source files, each
   source file gets its own copy of the template function definition.
   This leads to multiple definitions of the same function when the
   object files are linked together.

3. **Linker Errors**: Without the `inline` keyword, the linker sees
   multiple definitions of the same function and generates an error.
   The `inline` keyword allows the compiler to generate multiple
   definitions of the function but ensures that the linker treats them
   as a single definition.

 */