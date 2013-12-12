#ifndef VERIF_PLUGINS_EXCEPT_H
#define VERIF_PLUGINS_EXCEPT_H

#include <string>
#include <stdexcept>

struct Version {
	int major, minor;
};

namespace plugins {
	class PlugInNotFound : public std::exception {
		public:
			PlugInNotFound(const char *msg);
			PlugInNotFound(std::string &msg);
			virtual ~PlugInNotFound(void) throw();

			virtual const char* what(void) const throw();

		private:
			std::string error;
	};

	class PlugInWrongAPI : public std::exception {
		public:
			PlugInWrongAPI(const char *msg);
			PlugInWrongAPI(std::string &msg);
			virtual ~PlugInWrongAPI(void) throw();

			virtual const char* what(void) const throw();

		private:
			std::string error;
	};

	class PlugInError : public std::exception {
		public:
			PlugInError(const char *msg);
			PlugInError(std::string &msg);
			virtual ~PlugInError(void) throw();

			virtual const char* what(void) const throw();

		private:
			std::string error;
	};
}

#endif /* end of include guard */

