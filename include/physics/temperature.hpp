#ifndef CALC_STATS_HISTO_TEMPERATURE_HPP
#define CALC_STATS_HISTO_TEMPERATURE_HPP

#include <cmath>
#include <string>
#include <iostream>
#include <exception>
#include <initializer_list>

#include "physics/histogramme.hpp"
#include "physics/density.hpp"

namespace Stats {
	class Temperature : public Histogram {
		public:
			Temperature(io::types::Particules part, const int NbPart, const double r_min, const double r_max, const int bin=300, const double r_norm=1.0);
			virtual ~Temperature(void);

			const char* GetName(void);

			void Calcul(void);
			void Calcul(const Density &densite);

			using Histogram::operator();
			using Histogram::operator[];

		private:
			double t_moy = 0.;
	};

	class DensityTemperature : public Histogram {
		public:
			DensityTemperature(io::types::Particules part, const int NbPart, const double r_min, const double r_max, const int bin=300, const double r_norm=1.0);
			virtual ~DensityTemperature(void);

			const char* GetName(void);

			void Calcul(void);

			using Histogram::operator();
			using Histogram::operator[];

		private:
			double t_moy = 0.;
			Density densite;
	};
}

#endif /* end of include guard: CALC_STATS_HISTO_TEMPERATURE_HPP */
