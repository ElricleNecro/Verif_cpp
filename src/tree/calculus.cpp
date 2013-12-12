#include "tree/calculus.hpp"

namespace Physics {
	Calculus::Calculus(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			std::initializer_list<double> center,
			const double size
	) : actual(NULL)
	{
		this->actual = new Tree::OctTree(part, NbPart, NbMin, center, size);
		try {
			this->actual->Build();
		} catch(const Tree::BuildError &e) {
			std::cerr << "Exception caught in Calculus() while building tree: " << std::endl << e.what() << std::endl;
			throw;
		}
	}

	Calculus::Calculus(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			const double center[3],
			const double size
	) : actual(NULL)
	{
		this->actual = new Tree::OctTree(part, NbPart, NbMin, center, size);
		if( !this->actual->Build())
			throw this->actual->GetError();
	}

	Calculus::Calculus(const Tree::OctTree &obj) : actual(NULL)
	{
		this->actual = new Tree::OctTree(obj);
	}

	Calculus::Calculus(const Calculus &obj) : actual(obj.actual)
	{
	}

	Calculus::~Calculus(void)
	{
		delete this->actual;
	}

	std::string Calculus::GetError(void)
	{
		return this->actual->GetError();
	}

	double Calculus::cube_dist(Tree::OctTree *root, io::types::ParticuleData* part)
	{
		//double
			 //d1 = 0.0
		       //, d2 = 0.0
		       //, s  = root->GetSize()*0.5
		       //, d  = 0.
		       //, res = 0.
			//;
		//const double *p = root->GetCenter();

		//for(int i=0; i<3; i++)
		//{
			//d1 = part->Pos[i] - (p[i] - s);
			//d2 = part->Pos[i] - (p[i] + s);

			//if( d1 > 0.0 && d2 <= 0.0 )
				//d = 0.0;
			//else
				//d = std::fmin(std::fabs(d1), std::fabs(d2));
			//res += d*d;
		//}
		//return std::sqrt( d );

		double dx = 0.0,
		       dy = 0.0,
		       dz = 0.0,
		       d1 = 0.0,
		       d2 = 0.0;
		double s  = root->GetSize()*0.5;
		const double *p = root->GetCenter();

		d1 = part->Pos[0] - (p[0] - s);
		d2 = part->Pos[0] - (p[0] + s);

		if( d1 > 0.0 && d2 <= 0.0 )
			dx = 0.0;
		else
			dx = std::fmin(std::fabs(d1), std::fabs(d2));

		d1 = part->Pos[1] - (p[1] - s);
		d2 = part->Pos[1] - (p[1] + s);

		if( d1 > 0.0 && d2 <= 0.0 )
			dy = 0.0;
		else
			dy = std::fmin(std::fabs(d1), std::fabs(d2));

		d1 = part->Pos[2] - (p[2] - s);
		d2 = part->Pos[2] - (p[2] + s);

		if( d1 > 0.0 && d2 <= 0.0 )
			dz = 0.0;
		else
			dz = std::fmin(std::fabs(d1), std::fabs(d2));

		return std::sqrt( dx*dx + dy*dy + dz*dz );
	}

	void Calculus::Build(void)
	{
		if( !this->actual->Build())
			throw this->actual->GetError();
	}

	void Calculus::SetSwapFunction(Tree::SwapFunc *func)
	{
		this->actual->SetSwapFunction(func);
	}
}
