#include <iostream>
#include "include/astar.hpp"

int main(){
	std::cout << "hello world from a star path finding algorithm" << std::endl;

	Mapper mapper;

	mapper.LoadFile(std::string("./env/lab.bin"));

	mapper.FindTarget();

	mapper.Display();

	return 0;
}
