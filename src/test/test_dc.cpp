#include <fstream>
#include <random>
#include <cstdlib>
#include <iostream>

#include "tree_physics.hpp"

int comp(const void *a, const void *b);
void BruteForce(Physics::Classer* toto, Particule part_array, Particule part, const int NbPart, const int NbVois);

int comp(const void *a, const void *b)
{
	const Physics::Classer *ta = (const Physics::Classer*)a,
			       *tb = (const Physics::Classer*)b;

	if( ta->r - tb->r < 0 )
		return -1;
	else
		return 1;
}

void BruteForce(Physics::Classer* toto, Particule part_array, Particule part, const int NbPart, const int NbVois)
{
	Physics::Classer *tmp = 0x0;
	tmp = new Physics::Classer[NbPart];
	for(int i=0; i<NbPart; i++)
	{
		if( part_array[i].Id != part->Id )
		{
			tmp[i].r = std::sqrt( Physics::part_dist(&part_array[i], part));
			tmp[i].id = part_array[i].Id;
		}
		else
		{
			tmp[i].r = 3200.0;
			tmp[i].id = -6;
		}
	}

	std::qsort(tmp, NbPart, sizeof(Physics::Classer), comp);

	for(int i=0; i<NbVois; i++)
		toto[i] = {tmp[i].r, tmp[i].id};

	delete[] tmp;
}

int main(int argc, char const* argv[])
{
	int nb = 100, nb_vois=10;
	std::uniform_real_distribution<> d(-5., 5.);
	std::mt19937 gen(32);

	Particule part = NULL;
	part = (Particule)std::malloc(nb*sizeof(struct _particule_data));

	for(int i=0; i<nb; i++)
	{
		for(int j=0; j<3; j++)
			part[i].Pos[j] = d(gen);
		part[i].Id = i+1;
	}

	std::ofstream file("Test_dc_particule.dat");
	for(int i=0; i<nb; i++)
	{
		for(int j=0; j<3; j++)
			file << part[i].Pos[j] << " ";
		file << part[i].Id;
		file << std::endl;
	}
	file.close();

	Physics::DensityCenter tree(part, nb, 1, {0., 0., 0.}, 10.0, nb_vois);

	std::cout << "Finding " << nb_vois << " closest neighbor of particle " << part[0].Id << "." << std::endl;
	tree.FindNeighbor(&part[0]);


	Physics::Classer *brute = 0x0;
	brute = new Physics::Classer[nb_vois];

	for(int i=0; i<nb_vois; i++)
	{
		brute[i].id = -1;
		brute[i].r  = 10.0;
	}

	BruteForce(brute, part, &part[0], nb, nb_vois);


	for(int i=0; i<nb_vois; i++)
		std::cout << brute[i].id << " " << brute[i].r << "\t" << tree[i].id << " " << tree[i].r << std::endl;
	delete[] brute;

	file.open("Test_dc_vois.dat");
	if( !file.is_open() )
	{
		std::cerr << "File Test_dc_vois.dat not open!" << std::endl;
		return -1;
	}
	for(unsigned int i=0; i<tree.GetNbVois(); i++)
		file << tree[i].id << " " << tree[i].r << std::endl;
	file.close();

	std::free(part);

	(void)argc;
	(void)argv;

	return 0;
}
