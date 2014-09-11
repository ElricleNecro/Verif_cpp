#include "io/types.hpp"

#include <string>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <IOGadget/gadget.h>

#include "plugins/exceptions.hpp"
#include "io/reader_interface.hpp"

class Gadget : io::reader::PlugReader {
	public:
		Gadget(const std::string &name) : io::reader::PlugReader(name), particules(NULL), N(0), numfile(1), pot(false), acc(false), entropy(false), ts(false)
		{
		};

		virtual ~Gadget(void)
		{
			std::free(this->particules);
		};

		virtual void Read(void)
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
		};

		virtual io::types::ParticuleData* GetParticules(void)
		{
			return (io::types::ParticuleData*) this->particules;
		};

		virtual int NbPart(void)
		{
			this->N = 0;
			for(int i=0; i<6; i++)
				this->N += this->header.npart[i];
			return this->N;
		};

		virtual double Time(void)
		{
			return this->header.time;
		}
	private:
		iogadget::Particule_d particules;
		iogadget::Header header;
		int N, numfile;
		bool pot, acc, entropy, ts;
};

extern "C" {
	Version GetAPIVersion(void)
	{
		return Version{.major = 0, .minor = 1};
	}

	Gadget* Create(const char *fname)
	{
		return new Gadget(fname);
	}

	void Free(io::types::ParticuleData *part)
	{
		std::free(part);
	}
}

