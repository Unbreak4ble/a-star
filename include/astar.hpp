#pragma once

#include <thread>
#include <vector>
#include <string>
#include <memory>
#include "../utils/types.hpp"

enum class MapPixel {
	barrier = 0,
	path = 1,
	walked = 2,
	from = 3,
	target = 4,
	current = 5,
	undefined = -1,
};

typedef std::vector<std::vector<MapPixel>> Map;

class Mapper {
		std::vector<std::unique_ptr<std::thread>> pathTracers;
		std::vector<BYTE> map; // raw map from file
		Map parsed_map; // 2D parsed map

	public:
		bool found = false;
		unsigned long ref_count=0;

		Mapper();

		void LoadFile(std::string file);

		void FindTarget();

		void Display();

		std::vector<BYTE> GetRawMap();

		Map GetParsedMap();

		void setFound(Map new_map);

		void push_thread(std::unique_ptr<std::thread> th);
};

MapPixel toMapPixel(char num);

std::pair<int, int> indexToXY(Map map, size_t index);

size_t locateValue(Map map, MapPixel pixel);

size_t XYToIndex(Map map, int x, int y);

MapPixel getValue(Map map, int x, int y);

Map changePixelState(Map map, int x, int y, MapPixel new_state);

Map pathFindIt(Map map, void(*onRoute)(Map) = nullptr);

std::vector<std::pair<int,int>> findPathsAround(Map map, std::pair<int,int> xy, MapPixel pixel = MapPixel::path);

void forwardPath(Mapper *mapper, Map map, int stopped_x, int stopped_y);