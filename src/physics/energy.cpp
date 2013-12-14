#include "physics/energy.hpp"

namespace Calc {
	Energie::Energie(const Energie &obj) : Physics::Potential(obj)
	{
	}

	Energie::Energie(const Physics::Potential &obj) : Physics::Potential(obj)
	{
	}

	Energie::Energie(const Physics::Calculus &obj, const double opening) : Physics::Potential(obj, opening)
	{
	}

	Energie::Energie(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			const double center[3],
			const double size,
			const double opening,
			const double G
	) : Physics::Potential(part, NbPart, NbMin, center, size, opening, G)
	{
	}

	Energie::Energie(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			std::initializer_list<double> center,
			const double size,
			const double opening,
			const double G
	) : Physics::Potential(part, NbPart, NbMin, center, size, opening, G)
	{
	}

	Energie::~Energie(void)
	{
	}

	const char* Energie::GetName(void)
	{
		return "Energie";
	}

	double Energie::GetEc(void)
	{
		return this->Ec;
	}

	double Energie::GetEp(void)
	{
		return this->Ep;
	}

	double Energie::GetVirial(void)
	{
		return this->Vir;
	}

	void Energie::CalculAll(void)
	{
		this->Ep = 0.0;
		this->Ec = 0.0;
		for(int i=0; i<this->actual->GetN(); i++)
		{
			this->actual->GetPart()[i].Pot = this->GetPotential(&this->actual->GetPart()[i]);
			this->Ep += this->actual->GetPart()[i].Pot * this->actual->GetPart()[i].m;
			this->Ec += 0.5 * this->actual->GetPart()[i].m * Utils::v2(&this->actual->GetPart()[i]);
		}
		this->Ep *= 0.5;
		this->Vir = 2.0 * this->Ec / this->Ep;
	}

}
