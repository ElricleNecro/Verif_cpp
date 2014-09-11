#include "io/reader_interface.hpp"

namespace io {
	namespace reader {
		PlugReader::PlugReader(const std::string &name) : fname(name)
		{
		}

		PlugReader::~PlugReader(void)
		{
		}

		void PlugReader::Read(void)
		{
		}

		int PlugReader::NbPart(void)
		{
			std::cout << "Dummy, D'oh!" << std::endl;
			return -1;
		}

		double PlugReader::Time(void)
		{
			return 0.;
		}

		types::Particules PlugReader::GetParticules(void)
		{
			return NULL;
		}
	}
}

