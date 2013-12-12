#include <cmath>
#include <ctime>
#include <random>
#include <cstdlib>
#include <iostream>

#include "cli/option.hpp"
#include "log/logger.hpp"

#include "io/types.hpp"
#include "io/reader.hpp"
#include "physics/physics.hpp"
#include "tree/densitycenter.hpp"

int comp(const void *a, const void *b);

class Application {
	public:
		Application(int argc, char *argv[]) : logger(std::cout), reader(), opts(argc, argv), NbPart(-1), swap(NULL)
		{
			std::cout << opts.infile[0] << std::endl;
			std::cout << opts.G << " " << opts.opening << opts.pos_conv << " " << opts.vit_conv << std::endl;
			std::cout << opts.outfile << std::endl;

			this->AddReader(opts.name);

			io::reader::PlugReader *file = reader.GetInstance(opts.name, "New")(opts.infile[0].c_str());
			//file.SetFromCLI()
			file->Read();
			this->particules = file->GetParticules();
			this->NbPart     = file->NbPart();
			this->swap       = reader.GetSwap(opts.name, "Swap");
			//this->free       = reader.GetFree(opts.name, "Free");

			std::qsort(this->particules, this->NbPart, sizeof(io::types::ParticuleData), this->comp);
		}

		~Application(void)
		{
			//this->free(this->particules);
		}

		void AddReader(const std::string name)
		{
			reader.Add(name);
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
			Physics::DensityCenter tree(
					this->particules,
					this->NbPart,
					15,
					{0., 0., 0.},
					2.*Utils::rayon(&this->particules[this->NbPart-1]),
					15
			);
			std::clock_t startTime = std::clock();
			io::types::ParticuleData center = tree.CalculAll();
			std::cout << double( std::clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;

			std::cout << "Found: ";
			for(int j=0; j<3; j++)
				std::cout << center.Pos[j] << " ";
			std::cout << std::endl;

			for(int i = 0; i < this->NbPart; i++)
			{
				for(int j = 0; j<3; j++)
				{
					this->particules[i].Pos[j] -= center.Pos[j];
					this->particules[i].Vit[j] -= center.Vit[j];
				}
			}

			return 0;
		}

	private:
		logging::Logger logger;
		io::reader::Reader reader;
		cli::DemoOptions opts;
		io::types::Particules particules;
		int NbPart;
		Tree::SwapFunc* swap;
		void (*free)(void*);
		//cfg::ConfigFile file;
};

int main(int argc, char * argv[])
{
	Application app(argc, argv);
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

