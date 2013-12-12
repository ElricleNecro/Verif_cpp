#include <iostream>

#include "io/gadget.hpp"

int main(int argc, char const* argv[])
{
	io::gadget::Read part("io_read_gadget.dat", 1);
	part.Read_File();
	std::cout << part << std::endl;
	//io::types::Particules my_part = (io::types::Particules)part;
	io::types::Particules my_part = part.GetPart();

	return 0;
}
