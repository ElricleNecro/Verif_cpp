#include "tree/potential.hpp"

namespace Physics {
	Potential::Potential(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			std::initializer_list<double> center,
			const double size,
			const double opening,
			const double G
	) : Calculus(part, NbPart, NbMin, center, size), opening(opening), G(G)
	{
	}

	Potential::Potential(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			const double center[3],
			const double size,
			const double opening,
			const double G
	) : Calculus(part, NbPart, NbMin, center, size), opening(opening), G(G)
	{
	}

	Potential::Potential(const Calculus &obj, const double opening) : Calculus(obj), opening(opening)
	{
	}

	Potential::Potential(const Potential &obj) : Calculus(obj), opening(obj.opening), G(obj.G), Ep(obj.Ep)
	{
	}

	Potential::~Potential(void)
	{
	}

	double Potential::GetOpening(void) const
	{
		return this->opening;
	}

	void Potential::SetOpening(const double opening)
	{
		this->opening = opening;
	}

	void Potential::SetG(const double G)
	{
		this->G = G;
	}

	double Potential::GetG(void) const
	{
		return this->G;
	}

	double Potential::GetEp(void) const
	{
		return this->Ep;
	}

	double Potential::exact(Tree::OctTree *root, io::types::ParticuleData *part)
	{
		double pot = 0.0, dist = 0.0;

		for(int i=0; i<root->GetN(); i++)
		{
			if( root->GetPart()[i].Id != part->Id )
			{
				dist = 0.0;
				for(int j=0; j<3; j++)
					dist += ( root->GetPart()[i].Pos[j] - part->Pos[j] ) * ( root->GetPart()[i].Pos[j] - part->Pos[j] );
				pot += this->G * root->GetPart()[i].m / std::sqrt( dist );
			}
		}

		return pot;
	}

	double Potential::approximate(Tree::OctTree *root, io::types::ParticuleData *part)
	{
		double dist = 0.0;
		for(int i=0; i<3; i++)
			dist += (root->GetMassCenter()[i] - part->Pos[i]) * (root->GetMassCenter()[i] - part->Pos[i]);
		return root->GetTotalMass() * this->G / std::sqrt( dist );
	}

	inline bool Potential::accept(Tree::OctTree *root, io::types::ParticuleData *part)
	{
		if( ( root->GetSize() / this->cube_dist(root, part) ) > this->opening )
			return true;
		return false;
	}

	double Potential::calc_potentiel(Tree::OctTree *root, io::types::ParticuleData *part)
	{
		if( this->accept(root, part) && root->Down() != NULL )
		{
			double pot = 0.0;
			for( auto t1 = root->Down(); t1 != NULL; t1 = t1->Next())
				pot += this->calc_potentiel(t1, part);
			return pot;
		}

		if( root->Down() != NULL )
			return this->approximate(root, part);
		else
			return this->exact(root, part);
	}

	void Potential::CalculAll(void)
	{
		this->Ep = 0.0;
		for(int i=0; i<this->actual->GetN(); i++)
		{
			this->actual->GetPart()[i].Pot = this->GetPotential(&this->actual->GetPart()[i]);
			this->Ep += this->actual->GetPart()[i].Pot * this->actual->GetPart()[i].m;
		}
		this->Ep *= 0.5;
	}

	double Potential::GetPotential(io::types::ParticuleData *part)
	{
		return this->calc_potentiel(this->actual, part);
	}
}

