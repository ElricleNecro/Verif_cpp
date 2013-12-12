#ifndef TREE_UTILS_HPP
#define TREE_UTILS_HPP

#include <cmath>

#include "tree.hpp"

namespace Physics {
	double part_dist(io::types::ParticuleData *a, io::types::ParticuleData *b);

	struct Classer {
		double r;
		int id;

		static void Swap(Classer &a, Classer &b);
	};
} // namespace Physics

#endif /* end of include guard */
