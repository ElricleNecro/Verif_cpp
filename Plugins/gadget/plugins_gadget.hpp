#ifndef VERIF_GADGET_IO_H
#define VERIF_GADGET_IO_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <IOGadget/gadget.h>

#include "io/types.hpp"
#include "cli/option.hpp"
#include "plugins/exceptions.hpp"
#include "io/reader_interface.hpp"

class Gadget : io::reader::PlugReader {
	public:
		Gadget(const std::string &fname);
		virtual ~Gadget(void);

		virtual void SetFromCLI(const cli::DemoOptions &opt);

		virtual void Read(void);
		virtual io::types::ParticuleData* GetParticules(void);
		//virtual std::unique_ptr<io::types::ParticuleData[], void(*)(void*)> GetParticules(void);
		virtual int NbPart(void);

	private:
		iogadget::Particule particules;
		iogadget::Header header;
		int N, numfile;
		bool pot, acc, entropy, ts;
};

extern "C" {
	Version GetAPIVersion(void);
	Gadget* New(const char *fname);
	void Swap(iogadget::Particule a, iogadget::Particule b);
}

#endif /* end of include guard */
