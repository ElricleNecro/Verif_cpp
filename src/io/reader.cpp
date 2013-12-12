#include "io/reader.hpp"

namespace io {
	namespace reader {
		Reader::Reader(void)
		{
		}

		Reader::Reader(const std::string &file_name)
		{
			plug[file_name] = this->LoadPlugins(file_name);
		}

		Reader::~Reader(void)
		{
		}

		plugins::Plugins Reader::LoadPlugins(const std::string &file_name)
		{
			plugins::Plugins tmp(file_name);
			return tmp;
		}

		void Reader::SetLogger(logging::Logger &logy)
		{
			this->m_log = logy;
		}

		void Reader::Add(const std::string &file_name)
		{
			this->plug[file_name] = this->LoadPlugins(file_name);
		}

		NewReader* Reader::GetInstance(const std::string& name, const std::string& func)
		{
			return this->plug[name].GetFunction<io::reader::NewReader>(func.c_str());
		}

		Tree::SwapFunc* Reader::GetSwap(const std::string& name, const std::string& func)
		{
			return this->plug[name].GetFunction<Tree::SwapFunc>(func.c_str());
		}
	}
} // End of namespace io
