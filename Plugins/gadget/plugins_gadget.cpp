#include "plugins_gadget.hpp"

Version GetAPIVersion(void)
{
	return Version{.major = 0, .minor = 1};
}

Gadget* New(const char *fname)
{
	return new Gadget(fname);
}

Gadget::Gadget(const std::string &name) : io::reader::PlugReader(name), particules(NULL), N(0), numfile(1), pot(false), acc(false), entropy(false), ts(false)
{
}

Gadget::~Gadget(void)
{
	std::free(this->particules);
}

void Gadget::SetFromCLI(const cli::DemoOptions &opt)
{
	(void)opt;
}

void Gadget::Read(void)
{
	this->particules = iogadget::Double_Gadget_Read_format1(
			this->fname.c_str(),
			&this->header,
			this->numfile,
			this->pot,
			this->acc,
			this->entropy,
			this->ts
	);
	if( this->particules == 0x0 )
		throw plugins::PlugInError(iogadget::GetError());
}

int Gadget::NbPart(void)
{
	this->N = 0;
	for(int i=0; i<6; i++)
		this->N += this->header.npart[i];
	return this->N;
}

io::types::ParticuleData* Gadget::GetParticules(void)
{
	return (io::types::ParticuleData*) this->particules;
}

void Swap(iogadget::Particule_d a, iogadget::Particule_d b)
{
	iogadget::Double_Swap(a, b);
}

void Free(io::types::ParticuleData *part)
{
	std::free(part);
}
