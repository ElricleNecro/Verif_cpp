#ifndef LOG_LOGGER_HPP
#define LOG_LOGGER_HPP

#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

namespace logging {
	typedef std::string formatter_func(void);

	enum LogLevel {
		WARNING,
		ERROR,
		FATAL,

		DEBUG
	};

	class Logger
	{
		public:
			Logger(void);
			Logger(std::ostream &out);
			virtual ~Logger(void);

			void SetInstanceName(const std::string &prog_name);

			void SetOutput(std::ostream &out);
			std::ostream& GetOutput(void);

			std::ostream& Print(LogLevel flag);

		private:
			std::string format_date(std::tm *oh);

			std::ostream *out;
			std::string prog_name = "",
				    warn      = "34",
				    debug     = "32",
				    error     = "33",
				    fatal     = "31",
				    def       = warn;
	};
} // End of namespace log

#endif // LOG_LOGGER_HPP
