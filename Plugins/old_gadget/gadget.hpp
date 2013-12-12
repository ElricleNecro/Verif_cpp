#ifndef VERIF_GADGET_IO_H
#define VERIF_GADGET_IO_H

#include <string>
#include <iostream>
#include <stdexcept>

#include <IOGadget/gadget.h>

#include "define.h"
#include "io/types.hpp"
#include "cli/option.hpp"
#include "io/exception.hpp"

namespace io {
	namespace gadget {
		class Gadget : public iogadget::Header {
			public:
				//===========
				// Constructeur/Destructeur :
				//===========================
				Gadget(const std::string &fname);
				virtual ~Gadget(void);

				virtual io::types::Particules GetParticules(void);

				//==========
				// Accesseur :
				//============
				virtual types::Particules GetParticules(void);
				int GetNbPart(void) const;

				int GetFileType(void) const;
				void SetFileType(const int type);

				int GetNumFile(void) const;
				void SetNumFile(const int nfile);

				std::string GetFileName(void) const;

				void ReadPotential(void);
				void ReadAcceleration(void);
				void ReadRateEntropy(void);
				void ReadTimeStep(void);

				void NotReadPotential(void);
				void NotReadAcceleration(void);
				void NotReadRateEntropy(void);
				void NotReadTimeStep(void);

				//==========
				// Méthodes :
				//===========
				virtual void Read(void);

				//==========
				// Operateur :
				//============
				iogadget::_particule_data operator[](const int index) const;
				iogadget::_particule_data& operator[](const int index);
				//std::ostream& operator<<(std::ostream &obj);
				operator io::types::Particules(void);
				operator iogadget::Particule(void);

				//==========
				// Amis :
				//==========
				friend std::ostream& operator<<(std::ostream&, Read const&);

			private:
				io::types::Particules part;
				int ftype, NbPart, numfile;
				std::string fname;
				bool pot, acc, entropy, ts;
		};
	}
}

#endif /* end of include guard */
