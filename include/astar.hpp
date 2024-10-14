#pragma once

#include <thread>
#include <vector>
#include <string>
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
		std::vector<std::thread> pathTracers;
		std::vector<BYTE> map; // raw map from file
		Map parsed_map; // 2D parsed map

	public:
		bool found = false;

		Mapper();

		void LoadFile(std::string file);

		void FindTarget();

		void Display();

		std::vector<BYTE> GetRawMap();

		Map GetParsedMap();
};

MapPixel toMapPixel(char num);

Map pathFindIt(Map map, void(*onRoute)(Map) = nullptr);

void spawnTracers(Map map);

std::pair<int, int> indexToXY(Map map, size_t index);


size_t locateValue(Map map, MapPixel pixel);