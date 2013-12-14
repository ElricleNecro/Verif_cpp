#include "physics/histogramme.hpp"

namespace Stats {
	Histogram::Histogram(io::types::Particules part,
			const int NbPart,
			const double r_min,
			const double r_max,
			const int bin,
			const double r_norm
			) : part(part), NbPart(NbPart), NbBin(bin), r(NULL), bins(NULL), r_norm(r_norm), r_max(r_max), r_min(r_min)
	{
		this->CreateBins();

		this->bins = new double[this->NbBin];
		for(int i = 0; i < this->NbBin; i++)
			this->bins[i] = 0.;
	}

	Histogram::~Histogram(void)
	{
		delete[] this->r;
		delete[] this->bins;
	}

	const char* Histogram::GetName(void)
	{
		return "Histogram";
	}

	void Histogram::CreateBins(void)
	{
		this->r              = new double[this->NbBin+1];

		this->r[0]           = std::log10( this->r_min / this->r_norm );
		this->r[this->NbBin] = std::log10( this->r_max / this->r_norm );

		this->cte            = (this->r[this->NbBin] - this->r[0]) / this->NbBin;

		for(int i=this->NbBin; i>1; i--)
			this->r[i-1] = this->r[i] - this->cte;
	}

	int Histogram::GetNbBin(void) const
	{
		return this->NbBin;
	}

	void Histogram::SetNbBin(const int NbBin)
	{
		this->NbBin = NbBin;
		delete[] this->r;
		this->CreateBins();

		delete[] this->bins;
		for(int i = 0; i < this->NbBin; i++)
			this->bins[i] = 0.;
	}

	double Histogram::GetRMin(void) const
	{
		return this->r_min;
	}

	void Histogram::SetRMin(const double RMin)
	{
		this->r_min = RMin;
		delete[] this->r;
		this->CreateBins();
	}

	double Histogram::GetRMax(void) const
	{
		return this->r_max;
	}

	void Histogram::SetRMax(const double RMax)
	{
		this->r_max = RMax;
		delete[] this->r;
		this->CreateBins();
	}

	double Histogram::GetRNorm(void) const
	{
		return this->r_norm;
	}

	void Histogram::SetRNorm(const double RNorm)
	{
		this->r_norm = RNorm;
		delete[] this->r;
		this->CreateBins();
	}

	void Histogram::Calcul(void)
	{
	}

	double Histogram::operator() (const unsigned int a)
	{
		if( a > (unsigned int)(this->NbBin+1) )
			throw std::string("Invalid index!");
		return this->r[a];
	}

	double Histogram::operator[] (const unsigned int a) const
	{
		if( a > (unsigned int)(this->NbBin) )
			throw std::string("Invalid index!");
		return this->bins[a];
	}

} /* Stats */
