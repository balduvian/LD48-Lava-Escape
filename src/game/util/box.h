//
// Created by Emmet on 4/24/2021.
//

#ifndef CNGE_8_2_BOX_H
#define CNGE_8_2_BOX_H

namespace Game {
	struct Point {
		Point() = default;
		Point(i32);
		Point(i32 x, i32 y);
		Point(const Point & other) = default;
		auto operator==(const Point & other) const -> bool;
		auto operator!=(const Point & other) const -> bool;

		auto operator-(const Point & other) const -> Point;
		auto negative() const -> Point;

		auto rotateLeft() -> Point;
		auto rotateRight() -> Point;

		auto aligned(const Point & other) -> bool;
		auto ordinal() -> i32;

		static auto midPoint(Point &, Point &) -> Point;
		static auto midPoint(Point &&, Point &&) -> Point;

		constexpr static i32 X_TABLE[4] = {
			0, 1, 0, -1
		};

		constexpr static i32 Y_TABLE[4] = {
			1, 0, -1, 0,
		};

		constexpr static auto UP = 0;
		constexpr static auto RIGHT = 1;
		constexpr static auto DOWN = 2;
		constexpr static auto LEFT = 3;

		i32 x; i32 y;
	};

	struct Box {
		Box() = default;
		Box(i32, i32, i32, i32);
		auto operator=(const Box & other) -> Box &;

		Point points[4];

		constexpr static i32 INDICES_TABLE[4] = { 0, 2, 3, 1 };

		auto backLeft(i32) -> Point;
		auto forwardLeft(i32) -> Point;
		auto forwardRight(i32) -> Point;
		auto backRight(i32) -> Point;
	};
}

#endif //CNGE_8_2_BOX_H
