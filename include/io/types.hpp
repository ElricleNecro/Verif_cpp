#ifndef VERIF_TYPES_H
#define VERIF_TYPES_H

#include <memory>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace io {
	namespace types {
		struct ParticuleData {
			double Pos[3];
			double Vit[3];
			double m;
			double Pot;
			double Acc[3];
			double dAdt;
			double ts;
			double Rho;
			double U;
			double Ne;
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
