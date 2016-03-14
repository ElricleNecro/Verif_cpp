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
			PlugInNotFound(const PlugInNotFound &pinf);
			virtual ~PlugInNotFound(void) noexcept;

			virtual const char* what(void) const noexcept;

			PlugInNotFound& operator=(const PlugInNotFound &pinf);

		private:
			std::string error;
	};

	class PlugInWrongAPI : public std::exception {
		public:
			PlugInWrongAPI(const char *msg);
			PlugInWrongAPI(std::string &msg);
			PlugInWrongAPI(const PlugInWrongAPI &pinf);
			virtual ~PlugInWrongAPI(void) noexcept;

			virtual const char* what(void) const noexcept;

			PlugInWrongAPI& operator=(const PlugInWrongAPI &piwa);

		private:
			std::string error;
	};

	class PlugInError : public std::exception {
		public:
			PlugInError(const char *msg);
			PlugInError(std::string &msg);
			PlugInError(const PlugInError &pinf);
			virtual ~PlugInError(void) noexcept;

			virtual const char* what(void) const noexcept;

			PlugInError& operator=(const PlugInError &pie);

		private:
			std::string error;
	};
}

#endif /* end of include guard */

