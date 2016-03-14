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

	PlugInNotFound::PlugInNotFound(const PlugInNotFound &pinf) : error(pinf.error)
	{
	}

	PlugInNotFound::~PlugInNotFound(void) noexcept
	{
	}

	const char* PlugInNotFound::what(void) const noexcept
	{
		return this->error.c_str();
	}

	PlugInNotFound& PlugInNotFound::operator=(const PlugInNotFound &pinf)
	{
		if( this == &pinf )
			return *this;

		this->error = pinf.error;

		return *this;
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

	PlugInWrongAPI::PlugInWrongAPI(const PlugInWrongAPI &piwa) : error(piwa.error)
	{
	}

	PlugInWrongAPI::~PlugInWrongAPI(void) noexcept
	{
	}

	const char* PlugInWrongAPI::what(void) const noexcept
	{
		return this->error.c_str();
	}

	PlugInWrongAPI& PlugInWrongAPI::operator=(const PlugInWrongAPI &piwa)
	{
		if( this == &piwa )
			return *this;

		this->error = piwa.error;

		return *this;
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

	PlugInError::PlugInError(const PlugInError &pie) : error(pie.error)
	{
	}

	PlugInError::~PlugInError(void) noexcept
	{
	}

	const char* PlugInError::what(void) const noexcept
	{
		return this->error.c_str();
	}

	PlugInError& PlugInError::operator=(const PlugInError &pie)
	{
		if( this == &pie )
			return *this;

		this->error = pie.error;

		return *this;
	}
}
