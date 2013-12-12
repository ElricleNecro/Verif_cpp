#ifndef CALC_STATS_HISTO_DENSITY_HPP
#define CALC_STATS_HISTO_DENSITY_HPP

#include <cmath>
#include <string>
#include <iostream>
#include <exception>
#include <initializer_list>

#include "physics/histogramme.hpp"
#include "utils.hpp"

namespace Stats {
	class Density : public Histogram {
		public:
			Density(io::types::Particules part, const int NbPart, const double r_min, const double r_max, const int bin=300, const double r_norm=1.0);
			virtual ~Density(void);

			void Calcul(void);

			using Histogram::operator();
			using Histogram::operator[];
	};
}

#endif /* end of include guard */
