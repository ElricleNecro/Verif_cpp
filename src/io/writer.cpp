#include "io/writer.hpp"

namespace io {
	namespace writer {
		//Writer::Writer(void);
		//Writer::~Writer(void);
		//void Writer::LoadPlugins(const std::string &file_name);
		class HDF5Writer {
			HDF5Writer(const std::string& fname)
			: file(fname, H5F_ACC_TRUNC)
			{
			}

			HDF5Writer::~HDF5Writer(void)
			{
				this->file.close();
			}

			void HDF5Writer::Write(double *data, const int N, const char *fname)
			{
			}
		};
	}
}

