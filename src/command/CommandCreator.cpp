# include "CommandCreator.hpp"

// ++ Constructor
template<typename T>
CommandCreatorImpl<T>::CommandCreatorImpl(CommandType type, Server& server)
					: _type(type), _server(server) {}


/* Definition of the operator() function. 
	This function creates a new instance of type T using the _type 
	member variable and returns a pointer to it.
	You can use this function instead of calling a specific method */
template<typename T>
CommandPtr CommandCreatorImpl<T>::operator()() const {
	return new T(_type, _server);
}

/* This line explicitly instantiates the CommandCreatorImpl
	template for the Command type. Meaning that the compiler
	will generate the code for CommandCreatorImpl<Command>
	based on the template definition */
template class CommandCreatorImpl<Command>;
