#include <cmath>
#include <ctime>
#include <random>
#include <cstdlib>
#include <iostream>

#include <hdf5.h>
#include <boost/filesystem.hpp>

#include "cli/option.hpp"
#include "cfg/config.hpp"
#include "log/logger.hpp"

#include "io/types.hpp"
#include "io/reader.hpp"
#include "physics/physics.hpp"
#include "tree/densitycenter.hpp"

namespace fs = boost::filesystem;

typedef struct _ExtensibleDataSet {
	hsize_t size[2], offset[2];
	hid_t dataset, file, grp;
	bool incremente;
} *ExtensibleDataSet;

ExtensibleDataSet CreateExtensibleDS(hid_t id, const char *sub_grp, hsize_t dims[2]) //const int lig, const int col)
{
	ExtensibleDataSet  obj = (ExtensibleDataSet) malloc(sizeof( struct _ExtensibleDataSet ));
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
	ds             = H5Dcreate(id, sub_grp, H5T_IEEE_F64LE, dsp, H5P_DEFAULT, cparms, H5P_DEFAULT);

	// Enregistrement des propriétés du Dataset :
	obj->size[0]   = dims[0];
	obj->size[1]   = dims[1];
	obj->dataset   = ds;
	obj->file      = id;
	obj->offset[0] = 0;
	obj->offset[1] = 0;
	obj->incremente= false;

	H5Sclose(dsp);

	return obj;
	(void)status;
}

ExtensibleDataSet CreateExtensibleDS_integer(hid_t id, const char *sub_grp, hsize_t dims[2]) //const int lig, const int col)
{
	ExtensibleDataSet  obj = (ExtensibleDataSet) malloc(sizeof( struct _ExtensibleDataSet ));
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
	ds             = H5Dcreate(id, sub_grp, H5T_STD_I32LE, dsp, H5P_DEFAULT, cparms, H5P_DEFAULT);

	// Enregistrement des propriétés du Dataset :
	obj->size[0]   = dims[0];
	obj->size[1]   = dims[1];
	obj->dataset   = ds;
	obj->file      = id;
	obj->offset[0] = 0;
	obj->offset[1] = 0;
	obj->incremente= false;

	H5Sclose(dsp);

	return obj;
	(void)status;
}

void ExtensibleDataSet_Extend(ExtensibleDataSet id, const double *data, hsize_t dims[2])
{
	// On étend l'espace alloué au dataset :
	if( id->incremente || id->size[1] < dims[1] )
		id->size[1]     += dims[1];
	else
		id->incremente = true;
	herr_t status    = H5Dextend(id->dataset, id->size);
	// On récupère les infos sur l'espace nouvellement alloué :
	hid_t  filespace = H5Dget_space(id->dataset);
	// On sélectionne la zone où l'on va écrire les nouvelles données :
	status           = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, id->offset, NULL, dims, NULL);
	// On crée le dataspace pour les nouvelles données :
	hid_t  dataspace = H5Screate_simple(2, dims, NULL);

	// On écrit les données :
	status           = H5Dwrite(id->dataset, H5T_NATIVE_DOUBLE, dataspace, filespace, H5P_DEFAULT, data);

	// On incrémente l'offset :
	//id->offset[0]    += dims[0];
	id->offset[1]   += dims[1];

	// On libère la mémoire :
	H5Sclose(filespace);
	H5Sclose(dataspace);
	(void)status;
}

void ExtensibleDataSet_Extend_integer(ExtensibleDataSet id, const double *data, hsize_t dims[2])
{
	// On étend l'espace alloué au dataset :
	if( id->incremente || id->size[1] < dims[1] )
		id->size[1]     += dims[1];
	else
		id->incremente = true;
	herr_t status    = H5Dextend(id->dataset, id->size);
	// On récupère les infos sur l'espace nouvellement alloué :
	hid_t  filespace = H5Dget_space(id->dataset);
	// On sélectionne la zone où l'on va écrire les nouvelles données :
	status           = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, id->offset, NULL, dims, NULL);
	// On crée le dataspace pour les nouvelles données :
	hid_t  dataspace = H5Screate_simple(2, dims, NULL);

	// On écrit les données :
	status           = H5Dwrite(id->dataset, H5T_NATIVE_INT, dataspace, filespace, H5P_DEFAULT, data);

	// On incrémente l'offset :
	//id->offset[0]    += dims[0];
	id->offset[1]   += dims[1];

	// On libère la mémoire :
	H5Sclose(filespace);
	H5Sclose(dataspace);
	(void)status;
}

void ExtensibleDataSet_Close(ExtensibleDataSet id)
{
	H5Dclose(id->dataset);
}

int comp(const void *a, const void *b);

class ArgsChain
{
	private:
		std::map<std::string, std::string> corres;

	public:
		ArgsChain(void)
		{
			this->corres["-G"]          = "G";
			this->corres["--opening"]   = "opening";
			this->corres["-r"]          = "rayon";
			this->corres["--softening"] = "softening";
			this->corres["--pos-units"] = "pos_units";
			this->corres["--vel-units"] = "vel_units";
			this->corres["--norme"]     = "norme";
			this->corres["--log"]       = "logfile";
			this->corres["-o"]          = "outfile";
			this->corres["--reader"]    = "plug-ins";
			this->corres["--nb-bins"]   = "nb_bin";
			this->corres["-v"]          = "verbosity";
			this->corres["-l"]          = "leaf";
			this->corres["-c"]          = "config";
		}
		std::string operator()(std::string& var)
		{
			return this->corres[var];
		}
};

class Application {
	public:
		Application(
			const int argc,
			const char **argv
		) : logger(std::cout),
		    reader(NULL),
		    writer(NULL),
		    NbPart(-1),
		    // swap(NULL),
		    file(NULL)
		{
			cli::ArgumentParser<ArgsChain> parser(argc, argv);
			// Short options:
			parser.Add("-c")->UseArgs().Alias("--config").Set_Help("Configuration file to use.");
			parser.Add("-G")->UseArgs().Alias("--grav-constant").Set_Help("Value of the gravitationnal constant.");
			parser.Add("-r")->UseArgs().Alias("--rayon").Set_Help("Cut-off radius.");
			parser.Add("-l")->UseArgs().Alias("--leaf").Set_Help("Number of particles per leaf.");
			parser.Add("-v")->UseArgs().Alias("--verbosity").Set_Help("Verbosity level.");
			parser.Add("-o")->UseArgs().Alias("--outout").Set_Help("Output file.");

			// Long options:
			parser.Add("--pos-units")->UseArgs().Set_Help("Conversion factor for position.");
			parser.Add("--vel-units")->UseArgs().Set_Help("Conversion factor for velocities.");
			parser.Add("--reader")->UseArgs().Set_Help("Reader to use.");
			parser.Add("--softening")->UseArgs().Set_Help("Softening parameter for the potential calculation.");
			parser.Add("--opening")->UseArgs().Set_Help("Opening angle for the Peano-Hilbert criterion.");
			parser.Add("--log")->UseArgs().Set_Help("Log file.");
			parser.Add("--norme")->UseArgs().Set_Help("Normalisation parameter for the radius axis of statistical data.");
			parser.Add("--nb-bins")->UseArgs().Set_Help("Number of bin to use with the histograms.");

			cli::Config cfg_args = parser.Parse();

			cfg::ConfigReader config(INSTALL_DIR"/share/verif/config.yaml");
			YAML::Node tmp = YAML::convert<cli::Config>::encode(cfg_args);
			config.Add(tmp);
			this->opts = config.Get();

			this->opts.infile = parser.GetUnused();

			this->AddReader(opts.name);

			// this->init();
		}

		~Application(void)
		{
			this->clean();
		}

		void clean(void)
		{
			for(size_t i = 0, size = this->stats.size(); i < size; i++)
				delete this->stats[i];
			this->stats.clear();
			if( this->file != NULL )
				delete this->file;
			this->file = NULL;
		}

		void init(std::string& file)
		{
			this->file	 = this->Create(file.c_str());
			this->file->Read();
			this->particules = this->file->GetParticules();
			this->NbPart     = this->file->NbPart();

			this->SortByR();
			this->stats.push_back(
					new Stats::Density(
						this->particules,
						this->NbPart,
						Utils::rayon(
							&this->particules[0]
						),
						Utils::rayon(
							&this->particules[this->NbPart-1]
						),
						this->opts.nb_bin,
						this->opts.norme
					)
			);
			this->stats.push_back(
					new Stats::DensityTemperature(
						this->particules,
						this->NbPart,
						Utils::rayon(
							&this->particules[0]
						),
						Utils::rayon(
							&this->particules[this->NbPart-1]
						),
						this->opts.nb_bin,
						this->opts.norme
					)
			);
			this->stats.push_back(
					new Stats::Anisotropy(
						this->particules,
						this->NbPart,
						Utils::rayon(
							&this->particules[0]
						),
						Utils::rayon(
							&this->particules[this->NbPart-1]
						),
						this->opts.nb_bin,
						this->opts.norme
					)
			);
		}

		void AddReader(const std::string& name)
		{
			this->reader = new plugins::Plugins(name);
			this->Create = reader->GetFunction<io::reader::NewReader>("Create");
		}

		void SortByR(void)
		{
			std::qsort(this->particules, this->NbPart, sizeof(io::types::ParticuleData), this->comp);
		}

		static int comp(const void *a, const void *b)
		{
			const io::types::ParticuleData *ta = (const io::types::ParticuleData *) a,
						       *tb = (const io::types::ParticuleData *) b;
			double ra = Utils::rayon(ta),
			       rb = Utils::rayon(tb);

			if( ra > rb )
				return 1;
			else if( rb < ra )
				return -1;
			else
				return 0;
		}

		int main(void)
		{
			hid_t	grp,
				out_file = H5Fcreate(
						opts.outfile.c_str(),
						H5F_ACC_TRUNC,
						H5P_DEFAULT,
						H5P_DEFAULT
					);
			hsize_t size[2] = {1, 1};
			double val;
			ExtensibleDataSet eds;

			for(auto file: opts.infile)
			{
				grp = H5Gcreate(
						out_file,
						("/" + fs::basename(file)).c_str(),
						H5P_DEFAULT,
						H5P_DEFAULT,
						H5P_DEFAULT
				);
				eds = CreateExtensibleDS(
						out_file,
						("/" + fs::basename(file) + "/timeparam").c_str(),
						size
				);

				this->init(file);

				val = this->file->Time();
				ExtensibleDataSet_Extend(eds, &val, size);

				this->SortByR();
				Physics::DensityCenter dc(
						this->particules,
						this->NbPart,
						this->opts.leaf,
						{0., 0., 0.},
						2.*Utils::rayon(&this->particules[this->NbPart-1]),
						15
				);
				std::clock_t startTime = std::clock();
				io::types::ParticuleData center = dc.CalculAll_NewInit();
				std::cout << "Density center calculation time: " << double( std::clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;

				for(int i = 0; i < this->NbPart; i++)
				{
					for(int j = 0; j<3; j++)
					{
						this->particules[i].Pos[j] -= center.Pos[j];
						this->particules[i].Vit[j] -= center.Vit[j];
					}
				}

				this->SortByR();
				Calc::Energie pot(
						this->particules,
						this->NbPart,
						15,
						{0., 0., 0.},
						2.*Utils::rayon(&this->particules[this->NbPart-1]),
						0.5,
						opts.G
				);
				startTime = std::clock();
				pot.CalculAll();
				std::cout << "Potential calculation time: " << double( std::clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;

				val = pot.GetVirial();
				ExtensibleDataSet_Extend(eds, &val, size);

				val = pot.GetEc();
				ExtensibleDataSet_Extend(eds, &val, size);

				val = pot.GetEp();
				ExtensibleDataSet_Extend(eds, &val, size);

				std::cout << pot.GetVirial() << std::endl;
				std::cout << pot.GetEc() << "\t" << pot.GetEp() << std::endl;

				this->SortByR();
				for(size_t i = 0, size = this->stats.size(); i < size; i++)
					this->stats[i]->Calcul();

				this->clean();
			}

			return 0;
		}

	private:
		logging::Logger			 logger;
		// io::reader::Reader		 reader;
		plugins::Plugins		*reader;
		plugins::Plugins		*writer;
		io::reader::NewReader		*Create;
		cfg::Config			 opts;
		//cli::ArgsParser			 opts;
		io::types::Particules		 particules;
		int				 NbPart;
		// Tree::SwapFunc			*swap;
		io::reader::PlugReader		*file;
		//io::reader::FreeFunc		*free;
		std::vector<Stats::Histogram*>	 stats;
		//cfg::ConfigFile file;
};

int main(int argc, char ** argv)
{
	//Application app(argc, argv);
	//cli::ArgsParser args(argc, argv);
	Application app((const int)argc, (const char**)argv); //args.GetParameters());
	return app.main();

	int nb = 10;
	double *r = NULL;
	std::uniform_real_distribution<> d(-5., 5.);
	std::mt19937 gen;

	io::types::Particules part = (io::types::Particules)std::malloc(nb*sizeof(io::types::ParticuleData));
	r                        = new double[nb];

	for(int i=0; i<nb; i++)
	{
		r[i] = 0.;
		for(int j=0; j<3; j++)
		{
			part[i].Pos[j] = d(gen) + 5.;
			r[i]          += part[i].Pos[j]*part[i].Pos[j];
			part[i].Vit[j] = d(gen) + 5.;
			part[i].m      = 1.0;
		}
		part[i].Id = i+1;
		r[i]       = std::sqrt(r[i]);
	}
	std::qsort(r, nb, sizeof(double), comp);

	Physics::DensityCenter tree(part, nb, 1, {5., 5., 5.}, 10.0, 15);
	io::types::ParticuleData center = tree.CalculAll();

	std::cout << "Density Center:" << std::endl;
	for(int j = 0; j < 3; j++)
		std::cout << center.Pos[j] << "\t" << center.Vit[j] << std::endl;

	for(int i = 0; i < nb; i++)
		for(int j = 0; j < 3; j++)
		{
			part[i].Pos[j] -= center.Pos[j];
			part[i].Vit[j] -= center.Vit[j];
		}

	//Physics::Potential pot(part, nb, 1, {center.Pos[0], center.Pos[1], center.Pos[2]}, 2. * 10.0, 0.5);
	Calc::Energie pot(part, nb, 1, {center.Pos[0], center.Pos[1], center.Pos[2]}, 2. * 10.0, 0.5);
	pot.CalculAll();

	Stats::Density     densite(part, nb, r[0] - r[0]*0.01, r[nb-1], 5);
	Stats::Temperature temperature(part, nb, r[0] - r[0]*0.01, r[nb-1], 5);
	Stats::Anisotropy  anisotropy(part, nb, r[0] - r[0]*0.01, r[nb-1], 5);

	densite.Calcul();
	temperature.Calcul();
	temperature.Calcul(densite);
	anisotropy.Calcul();

	//std::free(part);
	delete[] r;

	(void)argc;
	(void)argv;

	return 0;
}

int comp(const void *a, const void *b)
{
	const double *ta = (const double*)a,
		     *tb = (const double*)b;

	if( *ta - *tb < 0 )
		return -1;
	else
		return 1;
}

