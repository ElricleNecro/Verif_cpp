#ifndef PHYSICS_HEADER_VERIF_H
#define PHYSICS_HEADER_VERIF_H

#include <cmath>
#include <string>
#include <iostream>
#include <exception>
#include <initializer_list>

#include "define.h"

#include "io/types.hpp"

#include "tree/potential.hpp"

#include "utils.hpp"

namespace Calc {
	class Energie : public Physics::Potential {
		public:
			Energie(io::types::Particules part, const int NbPart, const int NbMin, std::initializer_list<double> center, const double size, const double opening=0.5, const double G=6.67384e-11);
			Energie(io::types::Particules part, const int NbPart, const int NbMin, const double center[3], const double size, const double opening=0.5, const double G=6.67384e-11);
			Energie(const Physics::Calculus &obj, const double opening=0.5);
			Energie(const Physics::Potential &obj);
			Energie(const Energie &obj);
			~Energie(void);

			void CalculAll(void);

		private:
			double Ec  = 0.,
			       Ep  = 0.,
			       Vir = 0.;
	};
} // namespace Calc

#endif /* end of include guard */
