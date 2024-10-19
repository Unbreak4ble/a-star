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

	tracedMap = parsed_map;
}

void Mapper::FindTarget(){
	auto new_map = parsed_map;
	found = false;

	size_t index = locateValue(new_map, MapPixel::from);

	auto xy = indexToXY(new_map, index);

	std::vector<std::pair<int,int>> available_paths = findPathsAround(new_map, xy);

	std::vector<MapMapped> maps = {};
	std::vector<std::unique_ptr<std::thread>> root_ths;

	for(auto path : available_paths){
		auto current_maps = forwardPath(&root_ths, this, new_map, path.first, path.second);

		maps.insert(maps.begin(), current_maps.begin(), current_maps.end());
	}

	while(!found){
		backto:
		for(int i=0,j=0; i<maps.size(); i++, j++){
			MapMapped current_map = maps[i];

			std::pair<int,int> xy(current_map.last_x, current_map.last_y);

			std::vector<std::pair<int,int>> target_paths = findPathsAround(current_map.map, xy, MapPixel::target);

			if(target_paths.size() > 0){
				setFound(current_map.map);
				maps.erase(maps.begin(), maps.end());
				return;
			}

			// remove walked paths
			available_paths = findPathsAround(tracedMap, xy);
			if(available_paths.size() == 0){
				maps.erase(maps.begin()+i);
				i=i-1;
				continue;
			}

			available_paths = findPathsAround(current_map.map, xy);

			for(auto path : available_paths){
				bool duplicated = false;
				for(auto mapit : maps){
					if(mapit.last_x == path.first && mapit.last_y == path.second){
						maps.erase(maps.begin()+i);
						duplicated = true;
					}
				}

				if(duplicated)
					goto backto;

				auto current_maps = forwardPath(&root_ths, this, current_map.map, path.first, path.second);

				maps.insert(maps.end(), current_maps.begin(), current_maps.end());
			}
			
		}
	}
}

std::vector<MapMapped> forwardPath(std::vector<std::unique_ptr<std::thread>> *ths, Mapper *mapper, Map map, int stopped_x, int stopped_y){
	if(mapper->found) return {};
	if(mapper->ref_count >= REF_MAX) return {};

	std::vector<MapMapped> maps;

	map = changePixelState(map, stopped_x, stopped_y, MapPixel::walked);

	mapper->MergeWorkingMap(map);

	std::pair<int,int> xy(stopped_x, stopped_y);

	std::vector<std::pair<int,int>> target_paths = findPathsAround(map, xy, MapPixel::target);

	if(target_paths.size() > 0){
		mapper->setFound(map);
		return {};
	}

	std::vector<std::pair<int,int>> available_paths = findPathsAround(map, xy);

	for(auto path : available_paths){
		maps.push_back({
			.map=changePixelState(map, path.first, path.second, MapPixel::walked),
			.last_x = path.first,
			.last_y = path.second,
		});
	}

	return maps;
}

void Mapper::setFound(Map new_map){
	found_mtx.lock();
	if(found == true) return;

	parsed_map = new_map;
	found = true;
	found_mtx.unlock();
}

void Mapper::onStep(void(*callback)(Map map)){
	onStepEvent = callback;
}

void Mapper::Display(){
	displayMap(parsed_map);
}

std::vector<BYTE> Mapper::GetRawMap(){
	return map;
}

Map Mapper::GetParsedMap(){
	return parsed_map;
}


void Mapper::MergeWorkingMap(Map map){
	map_mtx.lock();
	
	tracedMap = mergeMaps(tracedMap, map);

	if(onStepEvent != nullptr){
		onStepEvent(tracedMap);
	}

	map_mtx.unlock();
}

Map mergeMaps(Map map1, Map map2){
	for(int y=0; y < map2.size(); y++){
		std::vector<MapPixel> line = map2.at(y);

		for(int x=0; x < line.size(); x++){
			MapPixel pixel = line.at(x);

			if(pixel == MapPixel::walked){
				map1[y][x] = pixel;
			}
		}
	}

	return map1;
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

void displayMap(Map map){
	for(int y=0; y<map.size(); y++){
		std::vector<MapPixel> line = map.at(y);

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


std::vector<std::pair<int,int>> findPathsAround(Map map, std::pair<int,int> from_xy, MapPixel pixel){
	std::vector<std::pair<int,int>> paths;
	int x = from_xy.first;
	int y = from_xy.second;

	for(int yy=-1; yy<2; yy++){
		for(int xx=-1; xx<2; xx++){
			short correlation = xx + yy;
			int xr = x+xx;
			int yr = y+yy;

			// ignore diagonal paths
			if(!(correlation == -1 || correlation == 1)) {
				continue;
			}

			MapPixel step = getValue(map, xr, yr);

			if(step == pixel){
				paths.push_back(std::pair<int,int>(xr,yr));
			}
		}
	}

	return paths;
}