#include <fstream>
#include <random>
#include <cstdlib>
#include <iostream>

#include "tree.hpp"

class TestTree : public Tree::OctTree {
	public:
		TestTree( Particule part,
				  const int NbPart,
				  const int NbMin,
				  std::initializer_list<double> center,
				  const double size
				) : Tree::OctTree(part, NbPart, NbMin, center, size)
		{
		};
		void SaveNode(const char *filename) {
			std::ofstream file(filename);

			WriteToNode(file, this);
		};
		void SaveNode(std::string filename) {
			std::ofstream file(filename.c_str());

			WriteToNode(file, this);
		};

	private:
		void WriteToNode(std::ofstream &file, Tree::OctTree *to_write) {
			std::cout << to_write->Down() << std::endl;
			if( to_write->Down() != NULL )
				this->WriteToNode(file, to_write->Down());
			else
			{
				std::cout << "Writing Level " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;

				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;

				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;

				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;

				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;

				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] - to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
				file << to_write->GetCenter()[0] - to_write->GetSize()/2.0 << " " << to_write->GetCenter()[1] + to_write->GetSize()/2.0 << " " << to_write->GetCenter()[2] + to_write->GetSize()/2.0 << " " << to_write->GetLevel() << std::endl;
			}

			if( to_write->Next() != NULL )
				this->WriteToNode(file, to_write->Next());
		};
};

int LvlMax(Tree::OctTree& tree)
{
	std::cout << tree.Down() << std::endl;
	if( tree.Down() == NULL )
		return tree.GetLevel();

	int lvl = 0;
	for(auto t1 = tree.Down(); t1 != NULL; t1 = t1->Next())
	{
		std::cout << t1 << std::endl;
		int tmp = LvlMax(*t1);
		if( tmp > lvl )
			lvl = tmp;
	}
	return lvl;
}

int main(int argc, char const* argv[])
{
	int nb = 100;
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

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
			std::cout << part[i].Pos[j] << " ";
		std::cout << std::endl;
	}

	Swap(&part[0], &part[1]);

	std::cout << "===========================================" << std::endl;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
			std::cout << part[i].Pos[j] << " ";
		std::cout << std::endl;
	}

	TestTree tree(part, nb, 1, {0., 0., 0.}, 10.0);
	if( !tree.Build() )
		std::cerr << tree.GetError() << std::endl;

	std::cout << "Most Deeper level is " << LvlMax(tree) << " Given by class: " << tree.GetDeep() << "." << std::endl;

	tree.SaveNode("Test.dat");

	std::ofstream file("Test_particule.dat");
	for(int i=0; i<nb; i++)
	{
		for(int j=0; j<3; j++)
			file << part[i].Pos[j] << " ";
		file << std::endl;
	}

	std::free(part);

	(void)argc;
	(void)argv;

	return 0;
}
