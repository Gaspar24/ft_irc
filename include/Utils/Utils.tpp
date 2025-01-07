#ifndef UTILS_TPP
# define UTILS_TPP

# include "Utils.hpp"
# include <cstdlib>
# include <ctime>
# include <string>
# include <map>

# ifndef DEBUG
#  define DEBUG 0
# endif

/**
 * @brief Converts an integer value to a string.
 * 
 * @param value The integer value to be converted.
 * @return std::string The string representation of the integer value.
 */
template <typename T>
std::string toStr(T const& value)
{
	std::ostringstream ss;
	ss << std::fixed << value;
	return ss.str();
}

/**
 * @brief Formats a given value into a string with specified width and
 * alignment.
 *
 * This function takes a value of any type, formats it into a string
 * with a specified width, and aligns it either to the left or right
 * based on the alignment flag.
 *
 * @tparam T The type of the value to be formatted.
 * @param value The value to be formatted.
 * @param width The width of the formatted string.
 * @param alignment If true, the value is left-aligned; if false, the
 * value is right-aligned.
 * @return A string representing the formatted value.
 */
template <typename T>
std::string formatValue(T value, int width, bool alignment)
{
	std::ostringstream oss;
	if (alignment)
		oss << std::left;
	else
		oss << std::right;
	oss << std::setw(width) << std::setfill(' ');
	oss << value;
	return (oss.str());
}

/**
 * @brief Generates a random value within a specified range.
 *
 * This function initializes the random seed and generates a random
 * value within the range of [-num, num). The generated value is then
 * cast to the specified type T.
 *
 * @tparam T The type to which the random value will be cast.
 * @param num The upper bound of the range. The random value will be
 * within the range of [-num, num).
 * @return T A random value of type T within the specified range.
 */
template <typename T>
inline T getRandomVal(size_t num)
{
	initSeed();
	double randomValue = static_cast<double>(rand()) / RAND_MAX * 2 * static_cast<double>(num) - static_cast<double>(num);
	return (static_cast<T>(randomValue));

}

/**
 * @brief Generates a random value of type size_t within the range [0, num).
 *
 * This function initializes the random seed and returns a random value
 * of type size_t that is within the range from 0 to num - 1.
 *
 * @tparam std::size_t The type of the random value to be generated.
 * @param num The upper bound (exclusive) for the random value.
 * @return A random value of type size_t within the range [0, num).
 */
template <>
inline size_t getRandomVal<std::size_t>(size_t num)
{
	initSeed();
	return (static_cast<size_t>(rand()) % num);
}


/**
 * @brief Generates a random string of a specified length.
 *
 * This template specialization of the getRandomVal function generates
 * a random string consisting of lowercase alphabetic characters. The
 * length of the string is determined by generating a random size_t
 * value and adding 1 to it.
 *
 * @tparam std::string The type of the value to be generated.
 * @param num The base number used to generate the length of the
 *            random string. This parameter is set to 10 within the
 *            function.
 * @return A randomly generated string of lowercase alphabetic
 * characters.
 */
template <>
inline std::string getRandomVal<std::string>(size_t num)
{
	std::string randomStr= "";
	size_t strLen;

	num = 10;
	strLen = getRandomVal<size_t>(num) + 1;
	for(size_t i = 0; i < strLen; i++)
	{
		char random_char = 'a' + static_cast<char>(rand() % 26);
		randomStr += random_char;
	}
	return (randomStr);
}

/**
 * @brief Specialization of getRandomVal template function for char
 * type.
 *
 * This function generates a random character within the ASCII range
 * of printable characters (from space ' ' to tilde '~'). It
 * initializes the random seed and then calculates a random value
 * within the specified range.
 *
 * @param num This parameter is ignored in the function and is always
 * set to 1.
 * @return A random printable ASCII character.
 */
template <>
inline char getRandomVal<char>(size_t num)
{
	initSeed();
	num = 1;
	unsigned int randomValue = 32 + (static_cast<unsigned int>(std::rand()) % (126 - 32 + static_cast<unsigned int>(num)));

	return (static_cast<char>(randomValue));
}

/**
 * @brief A functor for filling values with random numbers.
 *
 * This struct provides two operator overloads to generate random
 * values and assign them to a given variable. The random values are
 * generated within a specified maximum range.
 *
 * @tparam T The type of the values to be filled.
 */
template <typename T>
struct FillFunctor
{
	size_t max;
	FillFunctor(size_t m) : max(m) {}
	T operator()() const
	{
		return getRandomVal<int>(max);
	}
	void operator()(T& value) const
	{
		value = getRandomVal<int>(max);
	}
};

template <typename T>
struct PrintFunctor
{
	std::ostream& _os;
	T const& _s;
	size_t _size;
	typename T::value_type _toCompare;
	PrintFunctor(std::ostream& os, T const& s, size_t size, const typename T::value_type& toCompare = -1): _os(os), _s(s), _size(size), _toCompare(toCompare){}
	void operator()(const typename T::value_type& value) const
	{
		if (_size > 20)
		{
			static size_t limit;
			++limit;
			// std::cout << "(limit)";
			if (limit < 7 || limit > _size - 7)
				osPrint(_os, value);
			if (limit == 20)
			{
				std::cout << "[...] ";
			}
			if (limit == _size)
				limit = 0;
			else
				return ;
		}
		else
		{
			if (_toCompare > -1 && value == _toCompare)
				std::cerr << "*";
			osPrint(_os, value);
		}
	}
};

/**
 * @brief A functor for printing elements of a container to an output
 * stream.
 *
 * @tparam T The type of the container whose elements will be printed.
 */
// template <typename T>
// struct PrintFunctor
// {
// 	std::ostream& _os;
// 	T const& _s;
// 	PrintFunctor(std::ostream& os, T const& s): _os(os), _s(s){}
// 	void operator()(const typename T::value_type& value) const
// 	{
// 		osPrint(_os, value);
// 	}
// };

/**
 * @brief Functor to print the contents of a std::map.
 *
 * This struct defines a functor that prints each entry of a std::map
 * to a given output stream.
 *
 * @tparam K The type of the keys in the map.
 * @tparam V The type of the values in the map.
 */
template <typename K, typename V>
struct PrintFunctor<std::map<K, V> >
{
	std::ostream& _os;
	const std::map<K, V>& _container;

	PrintFunctor(std::ostream& os, const std::map<K, V>& container) : _os(os), _container(container) {}

	void operator()(const std::pair<const K, V>& entry) const {
		_os << entry.first << ": " << entry.second << std::endl;
	}
};
#endif // TUTILS_HPP