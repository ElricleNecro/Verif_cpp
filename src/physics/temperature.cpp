#include "physics/temperature.hpp"

namespace Stats {
	Temperature::Temperature(io::types::Particules part,
			const int NbPart,
			const double r_min,
			const double r_max,
			const int bin,
			const double r_norm
			) : Histogram(part, NbPart, r_min, r_max, bin, r_norm)
	{
		this->t_moy = 0.;
	}

	Temperature::~Temperature(void)
	{
	}

	const char* Temperature::GetName(void)
	{
		return "Temperature";
	}

	void Temperature::Calcul(void)
	{
		double  r        = 0.;
		int    *compteur = NULL;

		this->t_moy      = 0.0;
		compteur         = new int[this->NbBin];
		for(int i = 0; i<this->NbBin; i++)
		{
			compteur[i]   = 0;
			this->bins[i] = 0.;
		}

		for(int i = 0; i < this->NbPart; i++)
		{
			for(int j=0; j<this->NbBin; j++)
			{
				r = Utils::rayon(&part[i]);
				if( r > this->r[j] && r <= this->r[j+1] )
				{
					this->bins[j] += Utils::v2(&this->part[i]);
					compteur[j]++;
					break;
				}
			}
			this->t_moy += Utils::v2(&this->part[i]);
		}

		for(int i = 0; i < this->NbBin; i++)
			this->bins[i] /= (double)( (compteur[i] != 0)?compteur[i]:1.0 );

		this->t_moy /= this->NbPart;

		delete[] compteur;
	}

	void Temperature::Calcul(const Density &densite)
	{
		double d_all       = 0.0,
		       r           = 0.0;
		int   *compteur    = NULL;

		compteur    = new int[this->NbBin];
		for(int j=0; j<this->NbBin; j++)
		{
			compteur[j]   = 0;
			this->bins[j] = 0.;
		}

		for(int i = 0; i < this->NbPart; i++)
		{
			for(int j=0; j<this->NbBin; j++)
			{
				r = Utils::rayon(&part[i]);
				if( r > this->r[j] && r <= this->r[j+1] )
				{
					compteur[j]++;
					this->bins[j] += Utils::v2(&this->part[i]);
					break;
				}
			}
		}

		for(int i = 0; i < this->NbBin; i++)
		{
			this->bins[i] *= densite[i] / ( (double)( (compteur[i] != 0)?compteur[i]:1.0 ) );
			d_all         += densite[i];
		}

		this->t_moy = 0.0;
		for (int i = 0; i < this->NbBin; i++) {
			this->t_moy   += this->bins[i];
			this->bins[i] /= d_all;
		}
		this->t_moy /= d_all;

		delete[] compteur;
	}

	DensityTemperature::DensityTemperature(io::types::Particules part,
			const int NbPart,
			const double r_min,
			const double r_max,
			const int bin,
			const double r_norm
			) : Histogram(part, NbPart, r_min, r_max, bin, r_norm),
			    densite(part, NbPart, r_min, r_max, bin, r_norm)
	{
		this->t_moy = 0.;
	}

	DensityTemperature::~DensityTemperature(void)
	{
	}

	const char* DensityTemperature::GetName(void)
	{
		return "DensityTemperature";
	}

	void DensityTemperature::Calcul(void)
	{
		double d_all       = 0.0,
		       r           = 0.0;
		int   *compteur    = NULL;
		this->densite.Calcul();

		compteur    = new int[this->NbBin];
		for(int j=0; j<this->NbBin; j++)
		{
			compteur[j]   = 0;
			this->bins[j] = 0.;
		}

		for(int i = 0; i < this->NbPart; i++)
		{
			for(int j=0; j<this->NbBin; j++)
			{
				r = Utils::rayon(&part[i]);
				if( r > this->r[j] && r <= this->r[j+1] )
				{
					compteur[j]++;
					this->bins[j] += Utils::v2(&this->part[i]);
					break;
				}
			}
		}

		for(int i = 0; i < this->NbBin; i++)
		{
			this->bins[i] *= densite[i] / ( (double)( (compteur[i] != 0)?compteur[i]:1.0 ) );
			d_all         += densite[i];
		}

		this->t_moy = 0.0;
		for (int i = 0; i < this->NbBin; i++) {
			this->t_moy   += this->bins[i];
			this->bins[i] /= d_all;
		}
		this->t_moy /= d_all;

		delete[] compteur;
	}

} /* Stats */
