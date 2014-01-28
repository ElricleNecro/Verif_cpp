#ifndef VERIF_READER_IO_HPP
#define VERIF_READER_IO_HPP

#include <map>
#include <string>

#include "define.h"
#include "io/types.hpp"
#include "log/logger.hpp"
#include "plugins/Plugins.hpp"
#include "io/reader_interface.hpp"
#include "tree/tree.hpp"

namespace io {
	namespace reader {
		typedef PlugReader* NewReader(const char *name);
		typedef void FreeFunc(io::types::ParticuleData*);

		class Reader {
			public:
				Reader(void);
				Reader(const std::string &file_name);
				virtual ~Reader(void);

				void SetLogger(logging::Logger &logy);
				static plugins::Plugins LoadPlugins(const std::string &file_name);
				void Add(const std::string &file_name);
				NewReader* GetInstance(const std::string& name, const std::string& func);
				Tree::SwapFunc* GetSwap(const std::string& name, const std::string& func);
				FreeFunc* GetFree(const std::string& name, const std::string& func);

			private:
				std::map<std::string, plugins::Plugins> plug;
				logging::Logger m_log;
		};
	}
} // End of namespace io

#endif // VERIF_READER_IO_HPP
