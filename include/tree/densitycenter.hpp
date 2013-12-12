#ifndef TREE_PHYSICS_DENSITY_CENTER_HPP
#define TREE_PHYSICS_DENSITY_CENTER_HPP

#include <cmath>

#include "tree/tree.hpp"
#include "tree/calculus.hpp"
#include "tree/utils.hpp"
#include "utils.hpp"

namespace Physics {
	class DensityCenter : public Calculus {
		public:
			//******
			//Constructeurs :
			//***************
			DensityCenter(io::types::Particules part, const int NbPart, const int NbMin, std::initializer_list<double> center, const double size, const int NbVois);
			DensityCenter(io::types::Particules part, const int NbPart, const int NbMin, const double center[3], const double size, const int NbVois);
			DensityCenter(const Calculus &obj, const int NbVois);
			DensityCenter(const DensityCenter &obj);

			//******
			//Destructeur :
			//*************
			virtual ~DensityCenter(void);

			//******
			//Méthodes :
			//**********
			void FindNeighbor(io::types::ParticuleData* part);
			bool In(io::types::ParticuleData* part) const;
			io::types::ParticuleData CalculAll(void);

			//******
			//Assesseurs :
			//************
			unsigned int GetNbVois(void) const;
			void SetNbVois(const unsigned int);

			Classer* GetVois(void) const;

			//******
			//Opérateur :
			//***********
			Classer operator[](const int i) const;
			Classer& operator[](const int i);

		private:
			void search_neighbor(Tree::OctTree *node, io::types::ParticuleData* part);
			void calc_vois(Tree::OctTree *node, io::types::ParticuleData* part);
			void insert(Classer part);
			void InitVois(void);

			unsigned int NbVois;
			unsigned int EndVois;
			unsigned int BegInsert;
			Classer *vois;
	};
} // namespace Physics

#endif /* end of include guard */
