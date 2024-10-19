#include <iostream>
#include <unistd.h>
#include "include/astar.hpp"

void onstep(Map map){
	system("clear");displayMap(map);
	
	usleep(1000*10);
}

int main(){
	std::cout << "hello world from a star path finding algorithm" << std::endl;

	Mapper mapper;
	std::string file = "";

	std::cout << "enter path file: ";
	std::cin >> file;

	std::cout << "loading file" << std::endl;

	mapper.LoadFile(file);

	mapper.onStep(onstep);

	mapper.FindTarget();

	std::cout << "path finding done" << std::endl;

	mapper.Display();

	return 0;
}
