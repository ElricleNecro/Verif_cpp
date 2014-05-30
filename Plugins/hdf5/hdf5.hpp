#ifndef PLUGINS_HDF5_WRITER
#define PLUGINS_HDF5_WRITER

#include <hdf5.h>

//ExtensibleDataSet CreateExtensibleDS(hid_t id, const char *name, const char *sub_grp, const int lig, const int col);
//ExtensibleDataSet CreateExtensibleDS(hid_t id, const char *sub_grp, hsize_t dims[2]);
//void ExtensibleDataSet_Extend(ExtensibleDataSet id, const double *data, hsize_t dims[2]);

//ExtensibleDataSet CreateExtensibleDS_integer(hid_t id, const char *sub_grp, hsize_t dims[2]);
//void ExtensibleDataSet_Extend_integer(ExtensibleDataSet id, const double *data, hsize_t dims[2]);

//void ExtensibleDataSet_Close(ExtensibleDataSet id);

#include "cli/option.hpp"
#include "plugins/exceptions.hpp"
#include "io/writer_interface.hpp"

class ExtensibleDataSet {
	public:
		ExtensibleDataSet(hid_t file, const char *name); //, hsize_t dims[2] = {1., 1.});
		~ExtensibleDataSet(void);

		void SetShape(int a, int b=1);
		void Extend(double *data);

	private:
		hsize_t size[2], offset[2];
		hid_t dataset, file, grp;
		bool incremente;
};

class hdf5 {
	public:
		hdf5(const char *fname);
		~hdf5(void);

		bool write(IOWriter &obj);

	private:
		hid_t file;
};

#endif /* end of include guard: PLUGINS_HDF5_WRITER */
