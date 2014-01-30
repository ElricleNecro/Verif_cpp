#include "log/logger.hpp"

namespace logging {
	Logger::Logger(void) : out(&std::cout)
	{
	}

	Logger::Logger(Logger &lo) : out(lo.out)
	{
	}

	Logger::Logger(std::ostream &out) : out(&out)
	{
	}

	Logger::~Logger(void)
	{
	}

	Logger& Logger::operator=(Logger& logger)
	{
		this->out = logger.out;
		return *this;
	}

	void Logger::SetInstanceName(const std::string &prog_name)
	{
		this->prog_name = prog_name;
	}

	void Logger::SetOutput(std::ostream &out)
	{
		this->out = &out;
	}

	std::ostream& Logger::GetOutput(void)
	{
		return *this->out;
	}

	std::string Logger::format_date(std::tm *oh)
	{
		std::stringstream ss;
		ss << oh->tm_mday << "/" << oh->tm_mon << "/" << oh->tm_year << " : " << oh->tm_hour << "h " << oh->tm_min << "m " << oh->tm_sec << "s";
		return ss.str();
	}

	std::ostream& Logger::Print(LogLevel flag)
	{
		std::time_t t = std::time(nullptr);

		switch (flag)
		{
			case WARNING:
				*this->out << "\033[" << this->warn << "m" << "Warning: [" << this->format_date(std::localtime(&t)) << "]\033[00m ";
				break;

			case ERROR:
				*this->out << "\033[" << this->error << "m" << "Error: [" << this->format_date(std::localtime(&t)) << "]\033[00m ";
				break;

			case FATAL:
				*this->out << "\033[" << this->fatal << "m" << "Fatal: [" << this->format_date(std::localtime(&t)) << "]\033[00m ";
				break;

			case DEBUG:
				*this->out << "\033[" << this->debug << "m" << "Debug: [" << this->format_date(std::localtime(&t)) << "]\033[00m ";
				break;

			default:
				*this->out << "\033[" << this->def << "m" << "[" << this->format_date(std::localtime(&t)) << "]\033[00m ";
				break;
		}

		*this->out << this->prog_name << ": ";

		return *this->out;
	}
} // End of namespace log
