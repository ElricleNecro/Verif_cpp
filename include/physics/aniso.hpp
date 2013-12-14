#ifndef CALC_STATS_HISTO_ANISO_HPP
#define CALC_STATS_HISTO_ANISO_HPP

#include <cmath>
#include <string>
#include <iostream>
#include <exception>
#include <initializer_list>

#include "physics/histogramme.hpp"
#include "utils.hpp"

namespace Stats {
	class Anisotropy : public Histogram {
		public:
			Anisotropy(io::types::Particules part, const int NbPart, const double r_min, const double r_max, const int bin=300, const double r_norm=1.0);
			virtual ~Anisotropy(void);

			const char* GetName(void);

			double GetAnisotropy(void);

			void Calcul(void);

			using Histogram::operator();
			using Histogram::operator[];

		private:
			double Coeff = 0.;
	};
}

#endif /* end of include guard */
