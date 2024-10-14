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
	auto new_map = parsed_map;
	parsed_map = pathFindIt(new_map);
}

void Mapper::Display(){
	for(int y=0; y<parsed_map.size(); y++){
		std::vector<MapPixel> line = parsed_map.at(y);

		for(int x=0; x<line.size(); x++){
			MapPixel pixel = line.at(x);
         	std::string color = "97";

			switch(pixel){
        		case MapPixel::barrier:
        		color = "30";
        		break;
        		case MapPixel::path:
        		color = "37";
        		break;
        		case MapPixel::walked:
        		color = "31";
        		break;
        		case MapPixel::from:
        		color = "32";
        		break;
        		case MapPixel::target:
        		color = "33";
        		break;
        		case MapPixel::undefined:
        		color = "34";
        		break;
        	}

			std::cout << "\x1b[" << color << "m" << "◼" << "\033[0m" << (x != line.size()-1 ? " " : "");
		}

		std::cout << std::endl;
	}
}

std::vector<BYTE> Mapper::GetRawMap(){
	return map;
}

Map Mapper::GetParsedMap(){
	return parsed_map;
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

std::pair<int, int> indexToXY(Map map, size_t index){
	std::pair<int, int> pair(0,0);

	if(map.size() == 0) return pair;

	int vertical_size = map.size();
	int horizontal_size = map.at(0).size();
	int x=0;
	int y=0;

	if(index <= horizontal_size){
		pair.first = index;

		return pair;
	}

	for(;index > horizontal_size; y++, x=index-=horizontal_size);

	pair.first = x;
	pair.second = y;

	return pair;
}

Map pathFindIt(Map map, void(*onRoute)(Map)){
	std::cout << "path finding it" << std::endl;

	auto xy = indexToXY(map, 10);

	std::cout << "xy: " << xy.first << " | " << xy.second << std::endl;

	return map;
}