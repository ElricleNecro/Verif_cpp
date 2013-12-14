#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <initializer_list>

#include "io/types.hpp"

#include "define.h"

namespace Tree {
	class BuildError : public std::exception
	{
		public:
			BuildError(const char* Msg);
			BuildError(const std::string &Msg);
			virtual ~BuildError(void) throw();

			virtual const char* what(void) const throw();

		private:
			std::string msg;
	};

	typedef void SwapFunc(io::types::ParticuleData *a, io::types::ParticuleData *b);

	class OctTree {
		public:
			OctTree(io::types::Particules part,
				const int NbPart,
				const int NbMin,
				std::initializer_list<double> center,
				const double size,
				const unsigned int level=0,
				OctTree *parent=NULL
			       );
			OctTree(io::types::Particules part,
				const int NbPart,
				const int NbMin,
				const double center[3],
				const double size,
				const unsigned int level=0,
				OctTree *parent=NULL
			       );
			OctTree(const OctTree &obj);
			virtual ~OctTree(void);

			bool Build(void);

			// Node Mover :
			OctTree* Up(void);
			OctTree* Down(void);
			OctTree* Next(void);

			bool In(io::types::ParticuleData *part);
			OctTree* GetNodeOf(io::types::ParticuleData *part);

			// Some setter and getter :
			int    GetLevelMax(void) const;
			void   SetLevelMax(int newlevel);

			int SetPart(void);
			io::types::Particules GetPart(void);

			void SetSwapFunction(SwapFunc *func);

			const double* GetCenter(void) const;

			int GetN(void) const;
			int GetLevel(void) const;
			double GetSize(void) const;

			const double* GetMassCenter(void) const;
			double GetTotalMass(void) const;

			std::string GetError(void);

			int GetDeep(void) const;

		private:
			OctTree(const OctTree &obj, OctTree *parent);

			unsigned int level;		/*< Niveau du noeud dans l'arbre.*/
			double Pos[3];			/*< Coordonnées du centre géometrique du carré.*/
			double cote;			/*< Longueur du côté du carré.*/
			void build_level(void);
			std::string error = "";

		protected:
			OctTree *frere,
				*fils,
				*parent;
			io::types::Particules first;	/*< Pointeur vers la particule du tableau de particule.*/
			double cm[3];			/*< Position du centre de masse de la cellule.*/
			double M_cm;			/*< Masse totale dans la cellule.*/
			int N,				/*< Nombre de particule contenue dans le noeud.*/
			    Level_Max,
			    NB_Bro = 8,
			    Leaf;
			SwapFunc *Swap = NULL;
	};
}

#endif /* end of include guard */
