#ifndef CALC_STATS_HISTO_HPP
#define CALC_STATS_HISTO_HPP

#include <cmath>
#include <string>
#include <iostream>
#include <exception>
#include <initializer_list>

#include "io/types.hpp"

namespace Stats {
	class Histogram {
		public:
			Histogram(io::types::Particules part, const int NbPart, const double r_min, const double r_max, const int bin=300, const double r_norm=1.0);
			virtual ~Histogram(void);

			virtual int  GetNbBin(void) const;
			virtual void SetNbBin(const int NbBin);

			virtual double GetRMin(void) const;
			virtual void   SetRMin(const double RMin);

			virtual double GetRMax(void) const;
			virtual void   SetRMax(const double RMax);

			virtual double GetRNorm(void) const;
			virtual void   SetRNorm(const double RNorm);

			virtual void Calcul(void) = 0;

			double operator() (const unsigned int a);
			double operator[] (const unsigned int a) const;

		private:
			void CreateBins(void);

		protected:
			io::types::Particules part;
			int NbPart, NbBin;
			double *r      = NULL,
			       *bins   = NULL,
			       cte    = 1.0,
			       r_norm = 1.0,
			       r_max  = 1.0,
			       r_min  = 1.0;
	};
}

#endif /* end of include guard */
