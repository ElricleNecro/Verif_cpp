#ifndef UTILITAIRES_DIVERS_HPP
#define UTILITAIRES_DIVERS_HPP

#include <cmath>

#include "io/types.hpp"

namespace Utils {
	double rayon(const io::types::ParticuleData *part);
	double v2(const io::types::ParticuleData *part);
}

#endif /* end of include guard */
