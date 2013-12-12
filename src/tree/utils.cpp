#include "tree/utils.hpp"

namespace Physics {
	double part_dist(io::types::ParticuleData *a, io::types::ParticuleData *b)
	{
		double dist = 0.;

		for(int i=0; i<3; i++)
			dist += (a->Pos[i] - b->Pos[i]) * (a->Pos[i] - b->Pos[i]);

		//return std::sqrt( dist );
		return dist;
	}

	void Classer::Swap(Classer &a, Classer &b)
	{
		Classer tmp = {a.r, a.id};
		a = {b.r, b.id};
		b = {tmp.r, tmp.id};
	}
}
