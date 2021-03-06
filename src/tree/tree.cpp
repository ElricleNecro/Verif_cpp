#include "tree/tree.hpp"

namespace Tree {
	template<typename T>
	std::string convert(T number);

	template<typename T>
	std::string convert(T number)
	{
		std::stringstream ss;//create a stringstream
		ss << number;//add number to the stream
		return ss.str();//return a string with the contents of the stream
	}

	BuildError::BuildError(const char* Msg)
	: msg(Msg)
	{
	}

	BuildError::BuildError(const BuildError& old)
	: msg(old.msg)
	{
	}

	BuildError::BuildError(const std::string& Msg)
	: msg(Msg)
	{
	}

	BuildError::~BuildError(void) noexcept
	{
	}

	const char* BuildError::what(void) const noexcept
	{
		return this->msg.c_str();
	}

	OctTree::OctTree( io::types::Particules part,
			  const int NbPart,
			  const int NbMin,
			  std::initializer_list<double> center,
			  const double size,
			  const unsigned int level,
			  OctTree *parent
			) : level(level), cote(size), frere(NULL), fils(NULL), parent(parent), first(part), M_cm(0.), N(NbPart), Leaf(NbMin)
	{
		if( center.size() != 3 )
			throw std::string("Center Coordinate must be in 3D space");

		int j = 0;
		for(auto i = center.begin() ; i != center.end() && j < 3 ; ++i, j++) {
			this->Pos[j] = *i;
		}
	}

	OctTree::OctTree( io::types::Particules part,
			  const int NbPart,
			  const int NbMin,
			  const double center[3],
			  const double size,
			  const unsigned int level,
			  OctTree *parent
			) : level(level), cote(size), frere(NULL), fils(NULL), parent(parent), first(part), M_cm(0.), N(NbPart), Leaf(NbMin)
	{
		for(auto i = 0; i < 3; i++) {
			this->Pos[i] = center[i];
		}
	}

	OctTree::OctTree(const OctTree &obj,
			 OctTree *parent
			) : level(obj.level), cote(obj.cote), frere(NULL), fils(NULL), parent(parent), first(obj.first), M_cm(obj.M_cm), N(obj.N),
			    Level_Max(obj.Level_Max), NB_Bro(obj.NB_Bro), Leaf(obj.Leaf)
	{
		for(int i = 0; i<3; i++)
		{
			this->Pos[i] = obj.Pos[i];
			this->cm[i]  = obj.cm[i];
		}

		if( obj.fils != NULL )
			this->fils = new OctTree(*obj.fils, this);
		if( obj.frere != NULL )
			this->frere = new OctTree(*obj.frere, this);
	}

	OctTree::OctTree(const OctTree &obj) :  level(obj.level), cote(obj.cote), frere(NULL), fils(NULL), parent(NULL), first(obj.first), M_cm(obj.M_cm), N(obj.N),
						Level_Max(obj.Level_Max), NB_Bro(obj.NB_Bro), Leaf(obj.Leaf)
	{
		for(int i = 0; i<3; i++)
		{
			this->Pos[i] = obj.Pos[i];
			this->cm[i]  = obj.cm[i];
		}

		if( obj.fils != NULL )
			this->fils = new OctTree(*obj.fils, this);
		if( obj.frere != NULL )
			this->frere = new OctTree(*obj.frere, this);
	}

	bool OctTree::In(io::types::ParticuleData *part)
	{
		for(int i=0; i<3; i++) {
			if( part->Pos[i] <= this->Pos[i] - this->cote/2.0
			    ||
			    part->Pos[i] >  this->Pos[i] + this->cote/2.0
			  )
				return false;
		}

		return true;
	}

	std::string OctTree::GetError(void)
	{
		std::string tmp = this->error;
		this->error = "No Error.";
		return tmp;
	}

	int OctTree::GetDeep(void) const
	{
		if( this->fils == NULL )
			return this->level;

		int lvl = 0;
		for(auto t1=this->fils; t1 != NULL; t1 = t1->frere)
		{
			int tmp = t1->GetDeep();
			if( tmp > lvl )
				lvl = tmp;
		}
		return lvl;
	}

	OctTree* OctTree::Next(void)
	{
		return this->frere;
	}

	OctTree* OctTree::Up(void)
	{
		return this->parent;
	}

	OctTree* OctTree::Down(void)
	{
		return this->fils;
	}

	OctTree* OctTree::GetNodeOf(io::types::ParticuleData *part)
	{
		if( this->In(part) )
		{
			if( this->fils != NULL )
				return this->fils->GetNodeOf(part);
			else
				return this;
		}
		else
			if( this->frere != NULL )
				return this->frere->GetNodeOf(part);
		return NULL;
	}

	io::types::Particules OctTree::GetPart(void)
	{
		return io::types::Particules(this->first);
	}

	const double* OctTree::GetMassCenter(void) const
	{
		return (const double*)this->cm;
	}

	double OctTree::GetTotalMass(void) const
	{
		return this->M_cm;
	}

	const double* OctTree::GetCenter(void) const
	{
		return (const double*)this->Pos;
	}

	int OctTree::GetN(void) const
	{
		return this->N;
	}

	int OctTree::GetLevel(void) const
	{
		return this->level;
	}

	double OctTree::GetSize(void) const
	{
		return this->cote;
	}

	bool OctTree::Build(void)
	{
		if( this->first == NULL || this->N == 0 )
		{
			this->error = "No Particule array set!";
			return false;
		}

		if( this->frere != NULL )
			delete this->frere;
		if( this->fils != NULL )
			delete this->fils;

		this->build_level();
		return true;
	}

	void OctTree::build_level(void)
	{
		int       NbUsed  = 0;
		double    NewSide = this->cote * 0.25,
		          x_g     = 0.,
		          y_g     = 0.,
		          z_g     = 0.;
		OctTree **tmp     = &(this->fils);

		for(int i=0; i<this->NB_Bro; i++)
		{
			if( (i & 1) == 0 )
				x_g = this->Pos[0] + NewSide;
			else
				x_g = this->Pos[0] - NewSide;

			if( (i & 2) == 0 )
				y_g = this->Pos[1] + NewSide;
			else
				y_g = this->Pos[1] - NewSide;

			if( (i & 4) == 0 )
				z_g = this->Pos[2] + NewSide;
			else
				z_g = this->Pos[2] - NewSide;

			*tmp = new OctTree(
						&this->first[NbUsed],
						this->N - NbUsed,
						this->Leaf,
						{
							x_g,
							y_g,
							z_g
						},
						this->cote * 0.5,
						this->level + 1,
						this
					  );

			//int used = this->setPart(*tmp, to_build->N - NbUsed);
			int used = (*tmp)->SetPart();
			if( used != 0 )
			{
				//std::cout << "Son(" << this->level << ") " << i << ": " << tmp << " " << *tmp << " " << std::endl;
				(*tmp)->N = used;
				if( used > this->Leaf )
				{
					//if( !(*tmp)->build_level() )
					(*tmp)->Build();
				}
				NbUsed += used;
				tmp = &((*tmp)->frere);
			}
			else
			{
				delete *tmp;
				*tmp = NULL;
			}
		}

		if( NbUsed != this->N )
		{
			this->error = "Some Particles were not distributed at level " + convert<int>(this->level) + " : " + convert<int>(NbUsed) + ", " + convert<int>(this->N);
			std::string except = "Some Particles were not distributed at level " + convert<int>(this->level) +
					     ": " + convert<int>(NbUsed) + ", " + convert<int>(this->N) + "\n";
			//for(int i = NbUsed; i < this->N; i++)
			//{
				//except = except + "Particle " + convert<int>(i) + " ";
				//std::cerr << "\033[32m";
				//for(int j = 0; j < 3; j++)
				//{
					//std::cerr << this->first[i].Pos[j] << " (" << convert<float>(this->first[i].Pos[j]) << ")\t";
					//except = except + convert<float>(this->first[i].Pos[j]) + " ";
				//}
				//std::cerr << "\033[00m" << std::endl;
				//except = except + "\n";
			//}
			throw BuildError(except);
		}
	}

	int OctTree::SetPart(void)
	{
		int used = 0;

		for(int j=0; j<3; j++)
			this->cm[j] =0.;
		this->M_cm = 0.;

		for(int i=0; i<this->N; i++)
		{
			if( this->In(&this->first[i]) )
			{
				for(int j=0; j<3; j++)
					this->cm[j] += this->first[i].Pos[j];
				this->M_cm += this->first[i].m;

				if( i != used )
				{
					io::types::Swap( &this->first[i], &this->first[used]);
				}
				used++;
			}
		}

		for(int j=0; j<3; j++)
			this->cm[j] /= used;

		return used;
	}

	void OctTree::SetSwapFunction(SwapFunc *func)
	{
		this->Swap = func;

		std::cout << "in SetSwapFunction" << std::endl;

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
				std::cout << this->first[i].Pos[j] << " ";
			std::cout << std::endl;
		}

		this->Swap(&this->first[0], &this->first[1]);

		std::cout << std::endl;
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<3; j++)
				std::cout << this->first[i].Pos[j] << " ";
			std::cout << std::endl;
		}

	}

	OctTree::~OctTree(void)
	{
		if( this->frere != NULL )
			delete this->frere;
		if( this->fils != NULL )
			delete this->fils;
	}
} // namespace OctTree

//vim:spelllang=en
