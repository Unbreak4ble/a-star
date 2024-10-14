#pragma once

#include <vector>
#include <string>
#include "../utils/types.hpp"

enum class MapPixel {
	barrier = 0,
	path = 1,
	walked = 2,
	from = 3,
	target = 4,
};

class BinMap {
	std::vector<BYTE> map;
	std::vector<MapPixel> parsed_map;

	public:
		BinMap();

		std::vector<BYTE> GetMap();

		// stl friendly
		void LoadMap(std::string file);
};

class Mapper {
	BinMap map;

	public:
		Mapper();

		void loadFile(std::string file);

		void findTarget();

		std::vector<BYTE> getRawMap();

		std::vector<MapPixel> getParsedMap();
}

void display(BinMap map);
