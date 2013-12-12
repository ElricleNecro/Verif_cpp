#include "io/exception.hpp"

namespace io {
	Unsupported::Unsupported(const char *except) : msg(except)
	{
	}
	Unsupported::Unsupported(std::string except) : msg(except)
	{
	}

	Unsupported::~Unsupported(void) throw()
	{
	}

	const char* Unsupported::what(void) const throw()
	{
		return msg.c_str();
	}
}
