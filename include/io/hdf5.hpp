#ifndef HDF5_HPP_ZH5MWHI7
#define HDF5_HPP_ZH5MWHI7

#include <string>
#include <initializer_list>

#include <hdf5.h>
#include <boost/filesystem.hpp>

namespace io
{
	namespace hdf5
	{
		class DataSet
		{
			public:
				DataSet(hid_t file, std::string& sub_grp, hsize_t dims[2]);
				DataSet(hid_t file, std::string& sub_grp, std::initializer_list<hsize_t> dims);

				virtual ~DataSet(void);

			private:
				void init(hid_t file, std::string& sub_grp, hsize_t dims[2]);

				hsize_t size[2], offset[2];
				hid_t dataset, file;
				bool incr;
		};

		class Hdf5
		{
			public:
				Hdf5(std::string &fname, std::string &input);
				virtual ~Hdf5(void);

				DataSet* createDataSet(std::string &sub_grp);

			private:
				hid_t file;
				hid_t grp;
				std::string grp_name;
		};
	} /* hdf5 */
} /* io */

#endif /* end of include guard: HDF5_HPP_ZH5MWHI7 */
