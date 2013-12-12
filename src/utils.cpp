#include "utils.hpp"

namespace Utils {
	double rayon(const io::types::ParticuleData *part)
	{
		double dist = 0.;
		for(int j=0; j<3; j++)
			dist += part->Pos[j] * part->Pos[j];
		return std::sqrt( dist );
	}

	double v2(const io::types::ParticuleData *part)
	{
		double v = 0.;
		for(int i = 0; i < 3; i++)
			v += part->Vit[i] * part->Vit[i];
		return v;
	}
} // namespace Utils

