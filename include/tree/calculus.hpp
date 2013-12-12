#ifndef TREE_PHYSICS_HEADER_HPP
#define TREE_PHYSICS_HEADER_HPP

#include <cmath>

#include "tree.hpp"

namespace Physics {
	class Calculus {
		public:
			//******
			//Constructeurs :
			//***************
			Calculus(io::types::Particules part, const int NbPart, const int NbMin, std::initializer_list<double> center, const double size);
			Calculus(io::types::Particules part, const int NbPart, const int NbMin, const double center[3], const double size);
			Calculus(const Calculus &obj);
			Calculus(const Tree::OctTree &obj);

			//******
			//Destructeur :
			//*************
			virtual ~Calculus(void);

			//******
			//Méthodes :
			//**********
			virtual void Build(void);

			virtual double cube_dist(Tree::OctTree *node, io::types::ParticuleData *part);

			//******
			//Assesseurs :
			//************
			virtual std::string GetError(void);
			void SetSwapFunction(Tree::SwapFunc *func);

		protected:
			Tree::OctTree* actual;
	};
} // namespace Physics

#endif /* end of include guard */
