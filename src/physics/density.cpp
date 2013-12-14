#include "physics/density.hpp"

namespace Stats {
	Density::Density(io::types::Particules part,
			const int NbPart,
			const double r_min,
			const double r_max,
			const int bin,
			const double r_norm
			) : Histogram(part, NbPart, r_min, r_max, bin, r_norm)
	{
	}

	Density::~Density(void)
	{
	}

	const char* Density::GetName(void)
	{
		return "Density";
	}

	void Density::Calcul(void)
	{
		double r = 0;
		for(int i=0; i<this->NbPart; i++)
			for(int j=0; j<this->NbBin; j++)
			{
				r = Utils::rayon(&part[i]);
				if( r > this->r[j] && r <= this->r[j+1] )
					this->bins[j] += part[i].m;
			}

		for(int i=0; i<this->NbBin; i++)
			this->bins[i] /= (4.0 * M_PI *
					this->r_norm*this->r_norm*this->r_norm *		// In case of normalisation
					std::log(10.0) *				// Because we work in log10 bins
					this->r[i+1]*this->r[i+1]*this->r[i+1] *	// For the volume
					this->cte					// To get the density number.
					);
	}
} /* Stats */
