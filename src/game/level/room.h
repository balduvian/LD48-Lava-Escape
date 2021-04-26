//
// Created by Emmet on 4/23/2021.
//

#ifndef CNGE_8_2_ROOM_H
#define CNGE_8_2_ROOM_H

#include <vector>

#include "cnge/types.h"

#include "../util/vector.h"
#include "../util/box.h"
#include "block.h"

namespace Game {
	struct LinePair {
		LinePair() = default;
		LinePair(const LinePair & other) = default;

		LinePair(Point, Point, Point, Point);

		struct PositionsReturn {
			Vector left; Vector right;
		};

		auto getPoints(f32 along) -> PositionsReturn;

		Point startLeft, endLeft, startRight, endRight;
	};

	struct Section {
		Section() = default;
		Section(u32, u32, u32, u32, u32, u32);
		Section(const Section & other) = default;

		u32 x, y, width, height, indexX, indexY;

		std::vector<LinePair> linePairs;

		auto currentPair(f32 along) -> i32;
		auto currentAlong(f32 along) -> f32;

		auto smallSize() -> u32;
	};

	class Room {
	public:
		i32 x, y;
		u32 width, height;
		u32 sectionsWide, sectionsTall;

		Point playerSpawn;
		Point exitDirection;

		u32 sectionBuffer;

		std::vector<Section> sections;

		std::vector<Block> blocks;

		Room(i32, i32, u32, u32, u32, u32, u32);

		auto generate(Section * previousSection, Point incomingDirection) -> void;

		auto lastSection() -> Section *;

		auto getBlock(i32, i32) -> Block &;

	private:
		auto fillCircle(f32 centerX, f32 centerY, f32 radius, u32 blockType) -> void;
	};
}

#endif //CNGE_8_2_ROOM_H
