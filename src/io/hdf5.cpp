#include "io/hdf5.hpp"

namespace fs = boost::filesystem;

namespace io
{
	namespace hdf5
	{
		DataSet::DataSet(hid_t file, std::string& sub_grp, hsize_t dims[2])
		: dataset(0), file(0), incr(false)
		{
			this->init(file, sub_grp, dims);
		}

		DataSet::DataSet(hid_t file, std::string& sub_grp, std::initializer_list<hsize_t> dims)
		: dataset(0), file(0), incr(false)
		{
			if( dims.size() != 2 )
				throw std::string("DatasSet can only be 2-dimensional.");

			hsize_t ds_dims[2];

			int j = 0;
			for(auto i = dims.begin() ; i != dims.end() && j < 3 ; ++i, j++) {
				ds_dims[j] = *i;
			}

			this->init(file, sub_grp, ds_dims);
		}

		DataSet::~DataSet(void)
		{
			H5Dclose(this->dataset);
		}

		void DataSet::init(hid_t file, std::string& sub_grp, hsize_t dims[2])
		{
			hid_t              cparms;
			hid_t              ds,
					   dsp;
			hsize_t            maxdims[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
			herr_t             status;

			// Création du Chunk permettant d'insérer des colonnes ou lignes au fur et à mesure :
			cparms         = H5Pcreate(H5P_DATASET_CREATE);
			status         = H5Pset_chunk(cparms, 2, dims);

			// Création de l'espace de travail à allouer :
			dsp            = H5Screate_simple(2, dims, maxdims);

			// Création du dataset en lui-même :
			ds             = H5Dcreate(file, sub_grp.c_str(), H5T_IEEE_F64LE, dsp, H5P_DEFAULT, cparms, H5P_DEFAULT);

			// Enregistrement des propriétés du Dataset :
			this->size[0]   = dims[0];
			this->size[1]   = dims[1];
			this->dataset   = ds;
			this->file      = file;
			this->offset[0] = 0;
			this->offset[1] = 0;
			this->incr      = false;

			H5Sclose(dsp);
		}

		Hdf5::Hdf5(std::string &fname, std::string &input)
		{
			fs::path p(input);
			this->file = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

			this->grp_name = p.stem().string();
			this->grp  = H5Gcreate(this->file, this->grp_name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		}

		Hdf5::~Hdf5(void)
		{
			H5Gclose(this->grp);
			H5Fclose(this->file);
		}

		DataSet* Hdf5::createDataSet(std::string &sub_grp)
		{
			hsize_t dims[2] = {2L, 2L};
			std::string hierarchy = this->grp_name + "/" + sub_grp;
			DataSet *ds = new DataSet(this->file, hierarchy, dims);

			return ds;
		}
	} /* hdf5 */
} /* io */

