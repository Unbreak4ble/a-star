#pragma once

#include <thread>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include "../utils/types.hpp"

#define REF_MAX 20000

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

struct MapMapped {
	Map map;
	int last_x;
	int last_y;
};

class Mapper {
		std::vector<BYTE> map; // raw map from file
		Map parsed_map; // 2D parsed map
		Map tracedMap;
		std::vector<Map> maps_found;
		std::mutex found_mtx;
		void(*onStepEvent)(Map map) = nullptr;

	public:
		std::mutex ths_mtx;
		std::mutex map_mtx;
		std::atomic<bool> found;
		unsigned long ref_count=0;

		Mapper();

		void LoadFile(std::string file);

		void onStep(void(*callback)(Map map));

		void FindTarget();

		void Display();

		std::vector<BYTE> GetRawMap();

		Map GetParsedMap();

		void setFound(Map new_map);

		void addFound(Map new_map);

		void MergeWorkingMap(Map map);
};

Map mergeMaps(Map map1, Map map2);

void displayMap(Map map);

MapPixel toMapPixel(char num);

std::pair<int, int> indexToXY(Map map, size_t index);

size_t locateValue(Map map, MapPixel pixel);

size_t XYToIndex(Map map, int x, int y);

MapPixel getValue(Map map, int x, int y);

Map changePixelState(Map map, int x, int y, MapPixel new_state);

Map pathFindIt(Map map, void(*onRoute)(Map) = nullptr);

std::vector<std::pair<int,int>> findPathsAround(Map map, std::pair<int,int> xy, MapPixel pixel = MapPixel::path);

std::vector<MapMapped> forwardPath(std::vector<std::unique_ptr<std::thread>> *ths, Mapper *mapper, Map map, int stopped_x, int stopped_y);