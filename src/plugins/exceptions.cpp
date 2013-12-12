#include "plugins/exceptions.hpp"
namespace plugins {
	//--------------------------------------------------------------------------------
	// PlugInNotFound class
	//--------------------------------------------------------------------------------
	PlugInNotFound::PlugInNotFound(const char *msg) : error(msg)
	{
	}

	PlugInNotFound::PlugInNotFound(std::string &msg) : error(msg)
	{
	}

	PlugInNotFound::~PlugInNotFound(void) throw()
	{
	}

	const char* PlugInNotFound::what(void) const throw()
	{
		return this->error.c_str();
	}

	//--------------------------------------------------------------------------------
	// PlugInWrongAPI class
	//--------------------------------------------------------------------------------
	PlugInWrongAPI::PlugInWrongAPI(const char *msg) : error(msg)
	{
	}

	PlugInWrongAPI::PlugInWrongAPI(std::string &msg) : error(msg)
	{
	}

	PlugInWrongAPI::~PlugInWrongAPI(void) throw()
	{
	}

	const char* PlugInWrongAPI::what(void) const throw()
	{
		return this->error.c_str();
	}

	//--------------------------------------------------------------------------------
	// PlugInError class
	//--------------------------------------------------------------------------------
	PlugInError::PlugInError(const char *msg) : error(msg)
	{
	}

	PlugInError::PlugInError(std::string &msg) : error(msg)
	{
	}

	PlugInError::~PlugInError(void) throw()
	{
	}

	const char* PlugInError::what(void) const throw()
	{
		return this->error.c_str();
	}
}
