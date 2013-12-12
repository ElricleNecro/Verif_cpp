#include <fstream>
#include <random>
#include <cstdlib>
#include <iostream>

#include "tree_physics.hpp"

static double G;

double dist(const Particule a, const Particule b);
void BruteForce(Particule part, double * const pot, const int NbPart);

double dist(const Particule a, const Particule b)
{
	double r=0;
	for(int i=0; i<3; i++)
		r += (a->Pos[i] - b->Pos[i]) * (a->Pos[i] - b->Pos[i]);
	return std::sqrt( r );
}

void BruteForce(Particule part, double * const pot, const int NbPart)
{
	for(int i = 0; i < NbPart; i++) {
		for(int j = 0; j < NbPart; j++) {
			if( i != j )
				pot[i] += G*part[i].m * part[j].m / dist(&part[i], &part[j]);
		}
	}
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
		part[i].Pot = 0.0;
		part[i].m = 1.0;
	}

	std::ofstream file("Test_pot_particule.dat");
	for(int i=0; i<nb; i++)
	{
		for(int j=0; j<3; j++)
			file << part[i].Pos[j] << " ";
		file << part[i].Id;
		file << std::endl;
	}
	file.close();

	std::cout << "Tree initialisation... ";
	Physics::Potential tree(part, nb, 1, {0., 0., 0.}, 10.0, 0.0);
	std::cout << "Done!" << std::endl;

	std::cout << "Getting Gravitational constant... ";
	G = tree.GetG();
	std::cout << "Done!" << std::endl << "G = " << G << std::endl;

	std::cout << "Calculating potential... ";
	tree.CalculAll();
	std::cout << "Done!" << std::endl;

	double *brute = 0x0;
	brute = new double[nb];

	for(int i=0; i<nb_vois; i++)
		brute[i] = 0.;

	BruteForce(part, brute, nb);

	for(int i=0; i<nb_vois; i++)
		std::cout << brute[i] << "\t" << part[i].Pot << std::endl;
	delete[] brute;

	file.open("Test_dc_vois.dat");
	if( !file.is_open() )
	{
		std::cerr << "File Test_dc_vois.dat not open!" << std::endl;
		return -1;
	}
	for(int i=0; i<nb_vois; i++)
	{
		for(int j=0; j<3; j++)
			file << part[i].Pos[j] << " ";
		file << part[i].Pot << std::endl;
	}
	file.close();

	std::free(part);

	(void)argc;
	(void)argv;

	return 0;
}
