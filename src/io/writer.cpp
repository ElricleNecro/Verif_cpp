#include "io/writer.hpp"

namespace io {
	namespace writer {
		Writer::Writer(void)
		{
		}

		Writer::~Writer(void)
		{
		}

		plugins::Plugins Writer::LoadPlugins(const std::string &file_name)
		{
			plugins::Plugins tmp(file_name);
			return tmp;
		}

		void Writer::Add(const std::string &file_name)
		{
			this->PlugList[file_name] = this->LoadPlugins(file_name);
		}
	}
}

