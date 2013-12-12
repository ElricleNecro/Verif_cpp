#include "tree/densitycenter.hpp"

namespace Physics {
	DensityCenter::DensityCenter(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			const double center[3],
			const double size,
			const int NbVois
	) : Calculus(part, NbPart, NbMin, center, size), NbVois(NbVois), vois(NULL)
	{
		this->InitVois();
	}

	DensityCenter::DensityCenter(
			io::types::Particules part,
			const int NbPart,
			const int NbMin,
			std::initializer_list<double> center,
			const double size,
			const int NbVois
	) : Calculus(part, NbPart, NbMin, center, size), NbVois(NbVois), vois(NULL)
	{
		this->InitVois();
	}

	DensityCenter::DensityCenter(const Calculus &obj, const int NbVois) : Calculus(obj), NbVois(NbVois), vois(NULL)
	{
		this->InitVois();
	}

	DensityCenter::DensityCenter(const DensityCenter &obj) : Calculus(obj), NbVois(obj.NbVois), vois(NULL)
	{
		this->vois = new Classer[this->NbVois];
		for(unsigned int i=0; i<NbVois; i++) {
			this->vois[i].r  = obj.vois[i].r;
			this->vois[i].id = obj.vois[i].id;
		}
	}

	DensityCenter::~DensityCenter(void)
	{
		delete[] this->vois;
		this->vois = NULL;
	}

	void DensityCenter::InitVois(void)
	{
		if( this->vois != NULL )
			delete[] this->vois;

		this->vois   = new Classer[this->NbVois];

		for(unsigned int i=0; i<this->NbVois; i++) {
			this->vois[i].r  = this->actual->GetSize() * std::sqrt(3.);
			this->vois[i].id = -1;
		}
	}

	bool DensityCenter::In(io::types::ParticuleData* part) const
	{
		for(unsigned int i=0; i<this->NbVois; i++)
			if( this->vois[i].id == part->Id )
				return true;
		return false;
	}

	void DensityCenter::insert(Classer part)
	{
		this->vois[ this->NbVois - 1 ] = {part.r, part.id};
		for(int i=NbVois-2; i>=0; i--)
		{
			if( this->vois[i].r > this->vois[i+1].r )
				Classer::Swap(this->vois[i], this->vois[i+1]);
			else
				break;
		}
	}

	void DensityCenter::calc_vois(Tree::OctTree *root, io::types::ParticuleData* part)
	{
		double dmax = this->vois[this->NbVois - 1].r * this->vois[this->NbVois - 1].r;

		for(int i=0; i<root->GetN(); i++)
		{
			if( part_dist(&root->GetPart()[i], part) < dmax && part->Id != root->GetPart()[i].Id )
			{
#ifdef DEBUG_CALCVOIS
				if( this->In(&root->GetPart()[i]) )
				{
					for(unsigned int j=0; j<this->NbVois; j++)
						std::cout << "i: " << j << " " << this->vois[j].id << "\t" << root->GetPart()[i].Id << "\t" << part->Id << std::endl;
					throw std::runtime_error("Particule not supposed to be here!");
				}
#endif

				this->insert( Classer{
						std::sqrt(part_dist(&root->GetPart()[i], part)),
						root->GetPart()[i].Id
					}
				);
				dmax = this->vois[this->NbVois - 1].r * this->vois[this->NbVois - 1].r; //part_dist(&root->GetPart()[i], part);
			}
		}
	}

	void DensityCenter::search_neighbor(Tree::OctTree *search, io::types::ParticuleData* part)
	{
		if( this->cube_dist(search, part) > this->vois[this->NbVois - 1].r )
			return ;

		if( search->Down() != NULL )
			for(auto t1 = search->Down(); t1 != NULL; t1 = t1->Next())
				this->search_neighbor(t1, part);
		else
			this->calc_vois(search, part);
	}

	void DensityCenter::FindNeighbor(io::types::ParticuleData* part)
	{
		this->search_neighbor(this->actual, part);
	}

	io::types::ParticuleData DensityCenter::CalculAll(void)
	{
		double rho_loc     = 0.,
		       rho_loc_tot = 0.;
		io::types::ParticuleData a;

		for(int i=0; i<3; i++)
			a.Pos[i] = a.Vit[i] = 0.;

		for(int i = 0; i < this->actual->GetN(); i++)
		{
			this->InitVois();
			this->FindNeighbor(&this->actual->GetPart()[i]);
			rho_loc      = (this->NbVois - 1) * this->actual->GetPart()[i].m / ( (4./3.) * M_PI * (*this)[-1].r * (*this)[-1].r * (*this)[-1].r );
			rho_loc_tot += rho_loc;

			for(int j = 0; j < 3; j++)
			{
				a.Pos[j] += this->actual->GetPart()[i].Pos[j] * rho_loc;
				a.Vit[j] += this->actual->GetPart()[i].Vit[j] * rho_loc;
			}
		}

		for(int j = 0; j < 3; j++)
		{
			a.Pos[j] /= rho_loc_tot;
			a.Vit[j] /= rho_loc_tot;
		}

		return a;
	}

	Classer* DensityCenter::GetVois(void) const
	{
		return this->vois;
	}

	unsigned int DensityCenter::GetNbVois(void) const
	{
		return this->NbVois;
	}

	Classer DensityCenter::operator[](const int i) const
	{
		unsigned int indice = i;
		if( i < 0 )
			indice = this->NbVois + i;

		if( indice >= this->NbVois )
			throw std::string("Index Out of Bounds");
		return this->vois[indice];
	}

	Classer& DensityCenter::operator[](const int i)
	{
		unsigned int indice = i;
		if( i < 0 )
			indice = this->NbVois + i;

		if( indice >= this->NbVois )
			throw std::string("Index Out of Bounds");
		return this->vois[indice];
	}
}
