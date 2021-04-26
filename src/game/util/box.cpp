//
// Created by Emmet on 4/24/2021.
//

#include "cnge/math/math.h"

#include "box.h"

namespace Game {
	/* point */
	Point::Point(i32 ordinal) : x(X_TABLE[ordinal]), y(Y_TABLE[ordinal]) {}

	Point::Point(i32 x, i32 y) : x(x), y(y) {}

	auto Point::operator==(const Point &other) const -> bool {
		return x == other.x && y == other.y;
	}

	auto Point::operator!=(const Point &other) const -> bool {
		return x != other.x || y != other.y;
	}

	auto Point::operator-(const Point &other) const -> Point {
		return Point{ x - other.x, y - other.y };
	}

	auto Point::negative() const -> Point {
		return Point{ -x, -y };
	}

	auto Point::rotateLeft() -> Point {
		return Point(CNGE::mod(ordinal() - 1, 4));
	}

	auto Point::rotateRight() -> Point {
		return Point(CNGE::mod(ordinal() + 1, 4));
	}

	auto Point::midPoint(Point & p0, Point & p1) -> Point {
		return { (p0.x + p1.x) / 2, (p0.y + p1.y) / 2 };
	}

	auto Point::midPoint(Point && p0, Point && p1) -> Point {
		return { (p0.x + p1.x) / 2, (p0.y + p1.y) / 2 };
	}

	auto Point::aligned(const Point &other) -> bool {
		return x == other.x || y == other.y;
	}

	auto Point::ordinal() -> i32 {
		if (x == 0)
			if (y == 1)
				return UP;
			else
				return DOWN;
		else
			if (x == 1)
				return RIGHT;
			else
				return LEFT;
	}

	/* box */

	Box::Box(i32 x, i32 y, i32 width, i32 height) {
		points[0 * 2 + 0] = { x, y };
		points[0 * 2 + 1] = { x + width, y };
		points[1 * 2 + 0] = { x, y + height };
		points[1 * 2 + 1] = { x + width, y + height };
	}

	auto Box::operator=(const Box &other) -> Box & {
		points[0] = other.points[0];
		points[1] = other.points[1];
		points[2] = other.points[2];
		points[3] = other.points[3];

		return *this;
	}

	auto Box::backLeft(i32 ordinal) -> Point {
		return points[INDICES_TABLE[ordinal]];
	}

	auto Box::forwardLeft(i32 ordinal) -> Point {
		return points[INDICES_TABLE[CNGE::mod(ordinal + 1, 4)]];
	}

	auto Box::forwardRight(i32 ordinal) -> Point {
		return points[INDICES_TABLE[CNGE::mod(ordinal + 2, 4)]];
	}

	auto Box::backRight(i32 ordinal) -> Point {
		return points[INDICES_TABLE[CNGE::mod(ordinal + 3, 4)]];
	}
}
