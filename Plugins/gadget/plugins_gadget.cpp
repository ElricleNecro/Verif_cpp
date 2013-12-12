#include "plugins_gadget.hpp"

Version GetAPIVersion(void)
{
	return Version{.major = 0, .minor = 1};
}

Gadget* New(const char *fname)
{
	std::cout << "Getting file '" << fname << "'." << std::endl;
	return new Gadget(fname);
}

Gadget::Gadget(const std::string &name) : io::reader::PlugReader(name), particules(NULL), N(0), numfile(1), pot(false), acc(false), entropy(false), ts(false)
{
}

Gadget::~Gadget(void)
{
	//std::free(this->particules);
}

void Gadget::SetFromCLI(const cli::DemoOptions &opt)
{
	(void)opt;
}

void Gadget::Read(void)
{
	this->particules = Gadget_Read_format1(
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
	std::cout << "Settings Particle number!" << std::endl;
	this->N = 0;
	for(int i=0; i<6; i++)
		this->N += this->header.npart[i];
	return this->N;
}

//std::unique_ptr<io::types::ParticuleData[], void(*)(void*)> Gadget::GetParticules(void)
io::types::ParticuleData* Gadget::GetParticules(void)
{
	return (io::types::ParticuleData*) this->particules;
	//return std::unique_ptr<io::types::ParticuleData[], void(*)(void*)>(
			//(io::types::ParticuleData*)this->particules,
			//std::free
	//);
}

void Swap(iogadget::Particule a, iogadget::Particule b)
{
	iogadget::Swap(a, b);
}

//void Swap(Particule *a, Particule *b)
    //static void *(*ori_swap)(size_t) = NULL;
    //if (!ori_swap) {
        //if (!(ori_swap =
            //(void *(*)(Particule,Particule))dlsym(RTLD_NEXT,"Swap"))) {
            //perror("cannot fetch system malloc\n");
            //exit(1);
        //}
    //}
    //return ori_swap(a, b);
//}
