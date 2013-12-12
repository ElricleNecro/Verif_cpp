#include "io/types.hpp"

namespace io {
	namespace types {
		void Swap(Particules a, Particules b)
		{
			ParticuleData tmp;

			std::memcpy(&tmp, a, sizeof(tmp));
			std::memcpy(a, b, sizeof(tmp));
			std::memcpy(b, &tmp, sizeof(tmp));
		}
	}
}

