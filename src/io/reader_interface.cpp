#include "io/reader_interface.hpp"

namespace io {
	namespace reader {
		PlugReader::PlugReader(const std::string &name) : fname(name)
		{
		}

		PlugReader::~PlugReader(void)
		{
		}

		void PlugReader::SetFromCLI(const cli::Config &opt)
		{
			(void)opt;
		}

		void PlugReader::Read(void)
		{
		}

		int PlugReader::NbPart(void)
		{
			std::cout << "Dummy, D'oh!" << std::endl;
			return -1;
		}

		types::Particules PlugReader::GetParticules(void)
		{
			return NULL;
		}
	}
}

