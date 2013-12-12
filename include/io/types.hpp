#ifndef VERIF_TYPES_H
#define VERIF_TYPES_H

#include <memory>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace io {
	namespace types {
		struct ParticuleData {
			float Pos[3];
			float Vit[3];
			float m;
			float Pot;
			float Acc[3];
			float dAdt;
			float ts;
			float Rho;
			float U;
			float Ne;
			int Id;
			int Type;
		};

		typedef ParticuleData* Particules;
		typedef std::unique_ptr<io::types::ParticuleData[]> unique_particules;

		void Swap(Particules a, Particules b);

		//typedef iogadget::Particule Particules;
		//typedef iogadget::_particule_data ParticuleData;
	}
}

#endif /* end of include guard */
