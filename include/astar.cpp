#include "astar.hpp"
#include "../utils/fs.hpp"
#include <thread>
#include <fstream>
#include <iostream>
#include <unistd.h> 

static std::vector<std::thread> thds;

Mapper::Mapper(){
	//pathTracers = new std::vector<std::thread>();
}

void Mapper::push_thread(std::unique_ptr<std::thread> th){
	//pathTracers.push_back(th);
}

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

	size_t index = locateValue(new_map, MapPixel::from);

	auto xy = indexToXY(new_map, index);

	std::vector<std::pair<int,int>> available_paths = findPathsAround(new_map, xy);
	
	std::vector<std::unique_ptr<std::thread>> ths;

	for(auto path : available_paths){
		new_map = changePixelState(new_map, path.first, path.second, MapPixel::walked);

		ths.push_back(std::make_unique<std::thread>(std::thread(forwardPath, this, new_map, path.first, path.second)));
	}

	for(int i=0; i<ths.size(); i++){
		ths[i]->join();
	}
}

void Mapper::setFound(Map new_map){
	parsed_map = new_map;
	found = true;
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
        		case MapPixel::from:
        		color = "31";
        		break;
        		case MapPixel::target:
        		color = "32";
        		break;
        		case MapPixel::walked:
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
	int vertical_size = map.size();

	if(vertical_size == 0) return pair;

	int horizontal_size = map.at(0).size();

	pair.first = index%horizontal_size;
	pair.second = (index - pair.first)/horizontal_size;

	return pair;
}

size_t XYToIndex(Map map, int x, int y){
	int vertical_size = map.size();

	if(vertical_size == 0) return -1;

	int horizontal_size = map.at(0).size();
	
	return y*horizontal_size + x;
}

MapPixel getValue(Map map, int x, int y){
	int vertical_size = map.size();
	int horizontal_size = map.size() > 0 ? map[0].size() : 0;

	if(y < 0 || y >= vertical_size || x < 0 || x >= horizontal_size) return MapPixel::undefined;

	return map[y][x];
}

Map changePixelState(Map map, int x, int y, MapPixel new_state){
	map[y][x] = new_state;

	return map;
}

size_t locateValue(Map map, MapPixel pixel){
	for(int y=0; y < map.size(); y++){
		std::vector<MapPixel> line = map.at(y);

		for(int x=0; x < line.size(); x++){
			MapPixel pixel_0 = line.at(x);

			if(pixel_0 == pixel) return XYToIndex(map, x, y);
		}
	}

	return -1;
}

void forwardPath(Mapper *mapper, Map map, int stopped_x, int stopped_y){
	if(mapper->found) return;

	++(mapper->ref_count);

	std::pair<int,int> xy(stopped_x, stopped_y);

	std::vector<std::pair<int,int>> target_paths = findPathsAround(map, xy, MapPixel::target);

	if(target_paths.size() > 0){
		mapper->setFound(map);
		return;
	}

	std::vector<std::pair<int,int>> available_paths = findPathsAround(map, xy);

	std::vector<std::unique_ptr<std::thread>> ths;

	for(auto path : available_paths){
		map = changePixelState(map, path.first, path.second, MapPixel::walked);
		ths.push_back(std::make_unique<std::thread>(std::thread(forwardPath, mapper, map, path.first, path.second)));
	}

	for(int i=0; i<ths.size(); i++){
		ths[i]->join();
	}
}

std::vector<std::pair<int,int>> findPathsAround(Map map, std::pair<int,int> from_xy, MapPixel pixel){
	std::vector<std::pair<int,int>> paths;
	int x = from_xy.first;
	int y = from_xy.second;

	for(int yy=-1; yy<2; yy++){
		for(int xx=-1; xx<2; xx++){
			int xr = x+xx;
			int yr = y+yy;

			MapPixel step = getValue(map, xr, yr);

			if(step == pixel){
				paths.push_back(std::pair<int,int>(xr,yr));
			}
		}
	}

	return paths;
}