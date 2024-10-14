#include "astar.hpp"
#include "../utils/fs.hpp"
#include <thread>
#include <fstream>
#include <iostream>

Mapper::Mapper(){}

void Mapper::LoadFile(std::string file){
	std::string content = utils::fs::readFile(file);

	if(content.length() == 0) return;

	map.assign(content.begin(), content.end());

	parsed_map.erase(parsed_map.begin(), parsed_map.end());

	std::vector<MapPixel> line;

	for(char c : content){
		short num = c-0x30;
		
		if(num >= 0 && num <= 5){
			line.push_back(toMapPixel(num));
		}else if(c == 0xa && line.size() > 0){
			parsed_map.push_back(line);
			line.erase(line.begin(), line.end());
		}
	}

	if(line.size() > 0)
		parsed_map.push_back(line);
}

void Mapper::FindTarget(){

}

std::vector<BYTE> Mapper::GetRawMap(){

}

std::vector<MapPixel> Mapper::GetParsedMap(){

}

std::vector<BYTE> pathFindIt(std::vector<BYTE> map, void(*onRoute)(std::vector<BYTE>)){
	return {};
}

MapPixel toMapPixel(char num){
	switch(num){
		case 0:
			return MapPixel::barrier;
		break;
		case 1:
			return MapPixel::path;
		break;
		case 2:
			return MapPixel::walked;
		break;
		case 3:
			return MapPixel::from;
		break;
		case 4:
			return MapPixel::target;
		break;
		case 5:
			return MapPixel::current;
		break;
		default:
			return MapPixel::undefined;
		break;
	}
}

void display(std::vector<BYTE> map){

}
