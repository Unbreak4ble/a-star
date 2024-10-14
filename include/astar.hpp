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

class Mapper {
	std::vector<std::thread> pathTracers;
	std::vector<BYTE> map; // raw map from file
	std::vector<std::vector<MapPixel>> parsed_map; // 2D parsed map

	public:
		Mapper();

		void LoadFile(std::string file);

		void FindTarget();

		void Display();

		std::vector<BYTE> GetRawMap();

		std::vector<MapPixel> GetParsedMap();
};

MapPixel toMapPixel(char num);

std::vector<BYTE> pathFindIt(std::vector<BYTE> map, void(*onRoute)(std::vector<BYTE>) = nullptr);

void spawnTracers(std::vector<BYTE> map);

std::vector<MapPixel> parseMap(std::vector<BYTE> map);

void display(std::vector<BYTE> map);
