#ifndef TREE_CALCULUS_POTENTIAL_HPP
#define TREE_CALCULUS_POTENTIAL_HPP

#include <cmath>

#include "tree.hpp"
#include "calculus.hpp"

namespace Physics {
	class Potential : public Calculus {
		public:
			//******
			//Constructeurs :
			//***************
			Potential(io::types::Particules part, const int NbPart, const int NbMin, std::initializer_list<double> center, const double size, const double opening=0.5, const double G=6.67384e-11);
			Potential(io::types::Particules part, const int NbPart, const int NbMin, const double center[3], const double size, const double opening=0.5, const double G=6.67384e-11);
			Potential(const Calculus &obj, const double opening=0.5);
			Potential(const Potential &obj);

			//******
			//Destructeur :
			//*************
			virtual ~Potential(void);

			//******
			//Assesseurs :
			//************
			void SetG(const double G);
			double GetG(void) const;

			double GetOpening(void) const;
			void SetOpening(const double opening);

			double GetEp(void) const;

			//******
			//Méthodes :
			//**********
			double GetPotential(io::types::ParticuleData *part);

			void CalculAll(void);

		protected:
			virtual double approximate(Tree::OctTree *node, io::types::ParticuleData *part);

		private:
			inline bool accept(Tree::OctTree *node, io::types::ParticuleData* part);
			double exact(Tree::OctTree *node, io::types::ParticuleData* part);
			double calc_potentiel(Tree::OctTree *node, io::types::ParticuleData* part);

			double opening,
			       G  = 6.67384e-11,
			       Ep = 0.;
	};
} // namespace Physics

#endif /* end of include guard */
