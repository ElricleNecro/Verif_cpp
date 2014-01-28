#include "io/exception.hpp"

namespace io {
	Unsupported::Unsupported(const char *except) : msg(except)
	{
	}
	Unsupported::Unsupported(std::string except) : msg(except)
	{
	}

	Unsupported::~Unsupported(void) noexcept
	{
	}

	const char* Unsupported::what(void) const noexcept
	{
		return msg.c_str();
	}
}
