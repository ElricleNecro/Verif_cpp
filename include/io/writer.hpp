#ifndef VERIF_WRITER_IO_H
#define VERIF_WRITER_IO_H

#include <map>
#include <string>

#include "define.h"
#include "plugins/Plugins.hpp"

namespace io {
	namespace writer {
		class PlugWriter {
			public:
				PlugWriter(const char *fname);
				virtual ~PlugWriter(void);

				virtual void Write(double *data, const int N, const char *fname);
		};

		class Writer {
			public:
				Writer(void);
				~Writer(void);

				void LoadPlugins(const std::string &file_name);
				//New GetInstance(const std::string& func);

			private:
				std::map<std::string, plugins::Plugins> PlugList;
		};
	}
}

#endif /* end of include guard */
