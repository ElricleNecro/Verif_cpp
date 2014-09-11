#ifndef VERIF_READERINTERFACE_H
#define VERIF_READERINTERFACE_H

#include <string>
#include <iostream>

#include "define.h"

#include "io/types.hpp"

namespace io {
	namespace reader {
		class PlugReader {
			public:
				PlugReader(const std::string &fname);
				virtual ~PlugReader(void);

				virtual void Read(void);
				virtual types::Particules GetParticules(void);
				virtual int NbPart(void);
				virtual double Time(void);

			protected:
				std::string fname;
		};
	}
}

#endif /* end of include guard */
