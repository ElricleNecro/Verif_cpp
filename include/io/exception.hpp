#ifndef VERIF_EXCEPTION_IO_H
#define VERIF_EXCEPTION_IO_H

#include <string>
#include <exception>

namespace io {
	class Unsupported : std::exception {
		public:
			Unsupported(const char *except);
			Unsupported(std::string except);
			virtual ~Unsupported(void) throw();

			virtual const char* what(void) const throw();

		private:
			std::string msg;
	};
}

#endif /* end of include guard */
