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

std::vector<std::vector<MapPixel>> Mapper::GetParsedMap(){
	return parsed_map;
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
