//
// Created by Emmet on 4/23/2021.
//

#include <random>
#include <iostream>

#include "cnge/math/math.h"

#include "room.h"

namespace Game {
	Section::Section(u32 x, u32 y, u32 width, u32 height, u32 indexX, u32 indexY):
		x(x), y(y), width(width), height(height), indexX(indexX), indexY(indexY), linePairs() {}

	auto Section::currentPair(f32 along) -> i32 {
		if (along >= 1.0) {
			return linePairs.size() - 1;
		} else {
			return (i32) (along * linePairs.size());
		}
	}

	auto Section::currentAlong(f32 along) -> f32 {
		auto portion = 1.0_f32 / linePairs.size();

		return ((along - (currentPair(along) * portion)) / portion);
	}

	auto Section::smallSize() -> u32 {
		return width < height ? width : height;
	}

	Room::Room(i32 x, i32 y, u32 width, u32 height, u32 sectionsWide, u32 sectionsTall, u32 sectionBuffer) :
		x(x), y(y),
		width(width), height(height),
		sectionsWide(sectionsWide), sectionsTall(sectionsTall),
		playerSpawn(), exitDirection(),
		sectionBuffer(sectionBuffer),
		sections(),
		blocks(width * height)
	{}

	LinePair::LinePair(Point s0, Point e0, Point s1, Point e1) :
		startLeft(s0), endLeft(e0), startRight(s1), endRight(e1) {}

	auto LinePair::getPoints(f32 along) -> PositionsReturn {
		return {
			{
				CNGE::interp((f32) startLeft.x, (f32) endLeft.x, along),
				CNGE::interp((f32) startLeft.y, (f32) endLeft.y, along)
			},
			{
				CNGE::interp((f32) startRight.x, (f32) endRight.x, along),
				CNGE::interp((f32) startRight.y, (f32) endRight.y, along)
			}
		};
	}

	/* generator helper functions */

	constexpr auto NO_FORCE = -1;

	/**
	 *   --   --
	 * | [] | [] |
	 *   --   --
	 * | [] | [] |
	 *   --   --
	 *
	 * @return an array where every 2 * n is the left border of section n
	 * and every 2 * n + 1 is the right border of section n
	 */
	auto generateSectionIntervals(u32 size, u32 buffer, u32 number, i32 forceIndex, u32 forceSize, std::default_random_engine &random) {
		auto nonBufferSize = size - buffer * (number + 1);
		auto averageSize = nonBufferSize / number;

		auto deviation = std::uniform_int_distribution<i32>(0, averageSize / 2);

		auto dividers = std::vector<u32>(number + 1);
		for (auto i = 1; i < number; ++i)
			dividers[i] = (averageSize * i) + deviation(random) - averageSize / 4;

		dividers[0] = 0;
		dividers[number] = nonBufferSize;

		/* for the size at index */
		/*
		if (forceIndex != NO_FORCE) {
			auto currentSize = dividers[forceIndex + 1] - dividers[forceIndex];
			auto difference = forceSize - currentSize;

			/* can only expand/contract to the right
			if (forceIndex == 0) {
				dividers[1] += difference;

			/* can only expand/contract to the left
			}  else if (forceIndex == number - 1) {
				dividers[number - 1] -= difference;

			// can expand/cointract in both directions
			} else {
				auto halfDifference = difference / 2;
				dividers[forceIndex] -= halfDifference;
				dividers[forceIndex + 1] += difference - halfDifference;
			}
		}
		*/

		auto sectionSizes = std::vector<u32>(number * 2);
		for (auto i = 0; i < number; ++i) {
			auto previousDividers = buffer * (i + 1);

			sectionSizes[i * 2    ] = previousDividers + dividers[i];
			sectionSizes[i * 2 + 1] = previousDividers + dividers[i + 1];
		}

		return sectionSizes;
	}


	auto createMaze(i32 x, i32 y, u32 width, u32 height, bool canExitLeft, bool canExitRight, bool canExitDown, std::default_random_engine &random) {
		auto maze = std::vector<bool>(width * height);

		auto mazePath = std::vector<Point>();
		mazePath.emplace_back(x, y);
		maze[y * width + x] = true;

		while (true) {
			Point decisions[4];
			auto numDecisions = 0;

			/* can move up */
			if (y < height - 1 && !maze[(y + 1) * width + x])
				decisions[numDecisions++] = { 0, 1 };

			/* can move down */
			if ((y == 0 && canExitDown) || (y > 0 && !maze[(y - 1) * width + x]))
				decisions[numDecisions++] = { 0, -1 };

			/* can move left */
			if ((x == 0 && canExitLeft) || (x > 0 && !maze[y * width + (x - 1)]))
				decisions[numDecisions++] = { -1, 0 };

			/* can move right */
			if ((x == width - 1 && canExitRight) || (x < width - 1 && !maze[y * width + (x + 1)]))
				decisions[numDecisions++] = { 1, 0 };

			/* hit a dead end, backtrack */
			if (numDecisions == 0) {
				mazePath.pop_back();
				x = mazePath[mazePath.size() - 1].x;
				y = mazePath[mazePath.size() - 1].y;

			/* move to a new space in the maze */
			} else {
				auto decisionMaker = std::uniform_int_distribution<u32>(0, numDecisions - 1);
				auto move = decisions[decisionMaker(random)];

				x += move.x;
				y += move.y;

				if (x < 0 || x >= width || y < 0) {
					/* the last in the path is out of bounds, should not be used */
					/* except to determine end direction */
					mazePath.emplace_back(x, y);
					return mazePath;
				} else {
					maze[y * width + x] = true;
					mazePath.emplace_back(x, y);
				}
			}
		}
	}

	auto expandSection(Point direction, u32 & x, u32 & y, u32 & width, u32 & height, u32 buffer) {
		if (direction.y == 0) {
			width += buffer;
			if (direction.x < 0) x -= buffer;

		} else if (direction.x == 0) {
			height += buffer;
			if (direction.y < 0) y -= buffer;
		}
	}

	auto findNextPoints(Point startLeft, Point startRight, i32 ordinal, u32 boxIndex, std::vector<Box> & boxes, std::vector<Section> & sections) -> void {
		auto & box = boxes.at(boxIndex);
		auto & section = sections.at(boxIndex);

		/* the last box */
		if (boxIndex == boxes.size() - 1) {
			section.linePairs.emplace_back(startLeft, box.forwardLeft(ordinal), startRight, box.forwardRight(ordinal));

		/* recurse */
		} else {
			auto & nextBox = boxes[boxIndex + 1];

			auto leftEquals = box.forwardLeft(ordinal) == nextBox.backLeft(ordinal);
			auto rightEquals = box.forwardRight(ordinal) == nextBox.backRight(ordinal);

			Point endLeft, endRight;

			/* next box turns to the right */
			if (leftEquals && !rightEquals) {
				endLeft = nextBox.forwardLeft(ordinal);
				endRight = box.forwardRight(ordinal);
				ordinal = Point(ordinal).rotateRight().ordinal();

			/* next box turns to the left */
			} else if (rightEquals && !leftEquals) {
				endRight = nextBox.forwardRight(ordinal);
				endLeft = box.forwardLeft(ordinal);
				ordinal = Point(ordinal).rotateLeft().ordinal();

			/* next box goes straight */
			} else if (leftEquals && rightEquals) {
				endLeft = box.forwardLeft(ordinal);
				endRight = box.forwardRight(ordinal);

			/* box is adjacent */
			} else {
				/* box is adjacent to the left */
				if (box.forwardLeft(ordinal).aligned(nextBox.backRight(ordinal))) {
					endLeft = nextBox.backRight(ordinal);
					endRight = box.forwardRight(ordinal);
					section.linePairs.emplace_back(startLeft, endLeft, startRight, endRight);

					startLeft = endLeft;
					startRight = endRight;
					endLeft = nextBox.backLeft(ordinal);
					endRight = box.forwardLeft(ordinal);

				/* box is adjacent to the right */
				} else {
					endLeft = box.forwardLeft(ordinal);
					endRight = nextBox.backLeft(ordinal);
					section.linePairs.emplace_back(startLeft, endLeft, startRight, endRight);

					startLeft = endLeft;
					startRight = endRight;
					endLeft = box.forwardRight(ordinal);
					endRight = nextBox.backRight(ordinal);
				}
			}

			section.linePairs.emplace_back(startLeft, endLeft, startRight, endRight);

			findNextPoints(endLeft, endRight, ordinal, boxIndex + 1, boxes, sections);
		}
	}

	auto pointToLine(Point start, Point end, f32 x, f32 y) -> f32 {
		return abs((end.x - start.x) * (start.y - y) - (start.x - x) * (end.y - start.y)) / sqrt((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y));
	}

	auto inCircle(f32 centerX, f32 centerY, f32 radius, f32 x, f32 y) -> bool {
		return (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) < radius * radius;
	}

	auto Room::generate(Section * previousSection, Point incomingDirection) -> void {
		auto random = std::default_random_engine(std::random_device()());
		auto rangeSectionX = std::uniform_int_distribution<u32>(0, sectionsWide - 1);
		auto rangeSectionY = std::uniform_int_distribution<u32>(0, sectionsTall - 1);

		u32 startSectionX, startSectionY;
		bool canExitLeft, canExitRight, canExitDown;

		/* previous room coming down from above (previousSection can be null) */
		if (previousSection == nullptr || incomingDirection == Point { 0, -1 }) {
			startSectionX = previousSection == nullptr ? rangeSectionX(random) : previousSection->indexX;
			startSectionY = sectionsTall - 1;

			canExitLeft = startSectionX != 0;
			canExitRight = startSectionX != sectionsWide - 1;
			canExitDown = true;

		/* previous room coming into the right (from the left) */
		} else if (incomingDirection == Point { 1, 0 }) {
			startSectionX = 0;
			startSectionY = previousSection->indexY;

			canExitLeft = false;
			canExitRight = true;
			canExitDown = startSectionY != 0;

		/* previous room coming into the left (from the right) */
		} else {
			startSectionX = sectionsWide - 1;
			startSectionY = previousSection->indexY;

			canExitLeft = true;
			canExitRight = false;
			canExitDown = startSectionY != 0;
		}

		/* perform maze gen */
		auto path = createMaze(
			startSectionX,
			startSectionY,
			sectionsWide,
			sectionsTall,
			canExitLeft,
			canExitRight,
			canExitDown,
			random
		);

		/* choose sizes of sections */
		auto sectionXs = generateSectionIntervals(
			width,
			sectionBuffer,
			sectionsWide,
			/* if coming from the top,
			 * the starting section's width must match the previous room's ending section's width */
			previousSection != nullptr && incomingDirection == Point {0, -1 } ? startSectionX : NO_FORCE,
			previousSection != nullptr ? previousSection->width : 0,
			random
		);

		auto sectionYs = generateSectionIntervals(
			height,
			sectionBuffer,
			sectionsTall,
			/* if coming from the sides (not the top),
			 * the statrting section's height must match the previous room's ending section's height */
			previousSection != nullptr && incomingDirection != Point {0, -1 } ? startSectionY : NO_FORCE,
			previousSection != nullptr ? previousSection->height : 0,
			random
		);

		/* generate the internal connected sections */
		for (auto i = 0; i < path.size() - 1; ++i) {
			auto & space = path.at(i);

			/* direction the next section is from this section */
			auto direction = path[i + 1] - space;
			if (i == path.size() - 2) exitDirection = direction;

			/* section size without buffer included */
			auto sectionX = sectionXs[space.x * 2];
			auto sectionY = sectionYs[space.y * 2];
			auto sectionW = sectionXs[space.x * 2 + 1] - sectionXs[space.x * 2];
			auto sectionH = sectionYs[space.y * 2 + 1] - sectionYs[space.y * 2];

			/* include the buffer section in this section */
			expandSection(direction, sectionX, sectionY, sectionW, sectionH, sectionBuffer);

			/* special case for the first section */
			if (i == 0) {
				expandSection(incomingDirection.negative(), sectionX, sectionY, sectionW, sectionH, sectionBuffer);
			}

			sections.emplace_back(sectionX, sectionY, sectionW, sectionH, space.x, space.y);
		}

		/* determine the lava lines */
		auto boxes = std::vector<Box>(sections.size());
		for (auto i = 0; i < boxes.size(); ++i) {
			auto & section = sections.at(i);
			boxes[i] = Box((i32)section.x, (i32)section.y, (i32)section.width, (i32)section.height);
		}

		auto initialOrdinal = incomingDirection.ordinal();
		auto startLeft = boxes[0].backLeft(initialOrdinal);
		auto startRight = boxes[0].backRight(initialOrdinal);

		findNextPoints(startLeft, startRight, initialOrdinal, 0, boxes, sections);

		/* determine the cave block line */
		auto caveLine = std::vector<Point>(sections.size() + 2);
		for (auto i = 0; i < sections.size(); ++i) {
			auto & section = sections.at(i);
			caveLine[i + 1] = { (i32)section.x + (i32)section.width / 2,(i32)section.y + (i32)section.width / 2 };
		}

		caveLine[0] = Point::midPoint(startLeft, startRight);
		auto lastOrdinal = exitDirection.ordinal();
		caveLine[sections.size() + 1] = Point::midPoint(
			boxes[boxes.size() - 1].forwardLeft(lastOrdinal),
			boxes[boxes.size() - 1].forwardRight(lastOrdinal)
		);

		playerSpawn = caveLine[0];

		/* fill the whole room with bedrock */
		for (auto i = 0; i < width * height; ++i) {
			blocks[i] = Block::BEDROCK;
		}

		auto materialRange = std::uniform_int_distribution(Block::DIRT, Block::CRYSTAL);

		/* fill in air in sections */
		for (auto i = 0; i < sections.size(); ++i) {
			auto & section = sections.at(i);

			auto caveSize = section.smallSize() / 3;

			auto & point1 = caveLine[i];
			auto & point2 = caveLine[i + 1];
			auto & point3 = caveLine[i + 2];

			for (auto bx = section.x; bx < section.x + section.width; ++bx) {
				for (auto by = section.y; by < section.y + section.height; ++by) {
					auto xCenter = bx + 0.5_f32;
					auto yCenter = by + 0.5_f32;

					if (
						pointToLine(point1, point2, xCenter, yCenter) < caveSize ||
						pointToLine(point2, point3, xCenter, yCenter) < caveSize
					) {
						blocks[by * width + bx] = materialRange(random);
					}
				}
			}
		}

		/* starting and ending air pockets */
		fillCircle(caveLine[0].x, caveLine[0].y, sections[0].smallSize() / 2, Block::AIR);
		fillCircle(caveLine[caveLine.size() - 1].x, caveLine[caveLine.size() - 1].y, sections[sections.size() - 1].smallSize() / 2, Block::AIR);

		/* random air pockets */
		for (auto i = 0; i < sections.size(); ++i) {
			auto & section = sections.at(i);

			auto xRange = std::uniform_int_distribution<u32>(section.x, section.x + section.width);
			auto yRange = std::uniform_int_distribution<u32>(section.y, section.y + section.height);

			auto smallRadius = section.smallSize() / 2_f32;
			auto radiusRange = std::uniform_real_distribution<f32>(smallRadius * 0.25_f32, smallRadius);

			fillCircle(xRange(random), yRange(random), radiusRange(random), Block::AIR);
		}
	}

	auto Room::fillCircle(f32 centerX, f32 centerY, f32 radius, u32 blockType) -> void {
		/* create safe bounding box */
		auto left = (i32)floor(centerX - radius);
		if (left < 0) left = 0; else if (left >= width) left = width - 1;

		auto down = (i32)floor(centerY - radius);
		if (down < 0) down = 0; else if (down >= height) down = height - 1;

		auto up = (i32)ceil(centerY + radius);
		if (up < 0) up = 0; else if (up >= height) up = height - 1;

		auto right = (i32)ceil(centerX + radius);
		if (right < 0) right = 0; else if (right >= width) right = width - 1;

		for (auto bx = left; bx <= right; ++bx) {
			for (auto by = down; by <= up; ++by) {
				auto & block = blocks[by * width + bx];

				if (
					block.getType() != Block::BEDROCK &&
					(inCircle(centerX, centerY, radius, bx + 0.5_f32, by + 0.5_f32))
				) {
					block.setType(blockType);
				}
			}
		}
	}

	auto Room::lastSection() -> Section * {
		return sections.data() + sections.size() - 1;
	}

	auto Room::getBlock(i32 bx, i32 by) -> Block & {
		if (bx < 0) bx = 0;
		else if (bx >= width) bx = width - 1;

		if (by < 0) by = 0;
		else if (by >= height) by = height - 1;

		return blocks.at(by * width + bx);
	}
}
