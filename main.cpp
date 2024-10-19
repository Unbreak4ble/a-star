#include <iostream>
#include <unistd.h>
#include "include/astar.hpp"

void onstep(Map map){
	system("clear");displayMap(map);
	
	usleep(10000);
}

int main(){
	std::cout << "hello world from a star path finding algorithm" << std::endl;

	Mapper mapper;

	mapper.LoadFile(std::string("./env/big_lab.bin"));

	//mapper.Display();

	mapper.onStep(onstep);

	std::cout << "finding it" << std::endl;

	mapper.FindTarget();

	std::cout << "path finding done" << std::endl;

	mapper.Display();

	return 0;
}
