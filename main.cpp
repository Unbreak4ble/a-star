#include <iostream>
#include "include/astar.hpp"

int main(){
	std::cout << "hello world from a star path finding algorithm" << std::endl;

	Mapper mapper;

	mapper.LoadFile(std::string("./env/big_lab.bin"));

	mapper.Display();

	std::cout << "finding it" << std::endl;

	mapper.FindTarget();

	std::cout << "path finding done" << std::endl;

	mapper.Display();

	return 0;
}
