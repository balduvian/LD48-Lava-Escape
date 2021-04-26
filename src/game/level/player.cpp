//
// Created by Emmet on 4/24/2021.
//

#include <iostream>

#include "cnge/math/math.h"

#include "../gameResources.h"
#include "player.h"

namespace Game {
	Player::Player(f32 x, f32 y):
		x(x), y(y),
		velocityX(0), velocityY(0),
		facing(RIGHT),
		mineAnimation(0.2_f32),
		walkAnimation(0.5_f64), walkFrame(0),
		toolUsing(NO_TOOL),
		swingDirection(0),
		mineCooldown(0.5_f32),
		wallSiding(Point::DOWN),
		random(std::random_device()())
	{}

	constexpr auto FLOAT_SKEK = 0.00001_f32;

	constexpr auto COLL_X = true;
	constexpr auto COLL_Y = false;

	constexpr auto WHO_NONE = 0;
	constexpr auto WHO_X = 1;
	constexpr auto WHO_Y = 2;

	auto blockPos(f32 pos) -> i32 {
		return (i32)floor(pos);
	}

	/**
	 * finding the minimum block that you should start
	 * collision checking when moving in a positive direction
	 *
	 * if you are exactly on the low edge of a block, it puts you
	 * one block behind
	 */
	auto positiveBlockPos(f32 pos) -> i32 {
		/* on the boundary */
		if (floor(pos) == pos) {
			return blockPos(pos - 1);
		} else {
			return blockPos(pos);
		}
	}

	struct CollisionReturn {
		bool collision; f32 along;
	};

	auto collisionBlock(
		Room * room,
		bool x,
		i32 blockAxis,
		f32 wallAxis,
		Vector start,
		Vector end
	) -> CollisionReturn {
		auto along = CNGE::invInterp(x ? start.getX() : start.getY(), x ? end.getX() : end.getY(), wallAxis);
		auto wallOtherAxis = CNGE::interp(x ? start.getY() : start.getX(), x ? end.getY() : end.getX(), along);
		auto blockOtherAxis = blockPos(wallOtherAxis);

		/* collide */
		return {
			room->getBlock(
				(x ? blockAxis : blockOtherAxis) - room->x,
				(x ? blockOtherAxis : blockAxis) - room->y
			).getType() != Block::AIR,
			along
		};
	}

	/**
	 * @return should the mine loop stop
	 */
	auto Player::attemptMineBlock(Block & block, bool last) -> bool {
		if (block.getType() == Block::AIR) {
			if (last) mineAnimation.start();
			return false;
		}

		if (block.getType() == Block::BEDROCK) {
			mineAnimation.start();
			return true;
		}

		auto soundRange = std::uniform_int_distribution<i32>(0, 3);

		CNGE::Sound * breakSounds[4] = {
			&GameResources::break0,
			&GameResources::break1,
			&GameResources::break2,
			&GameResources::break3
		};

		CNGE::Sound * tingSounds[4] = {
			&GameResources::ting0,
			&GameResources::ting1,
			&GameResources::ting2,
			&GameResources::ting3
		};

		if (block.getType() == toolUsing) {
			mineAnimation.start();
			block.setType(Block::AIR);

			breakSounds[soundRange(random)]->play();

			return true;
		} else {
			mineCooldown.start();

			tingSounds[soundRange(random)]->play();

			return true;
		}
	}

	auto Player::update(Room * room, CNGE::Input * input, f64 time) -> void {
		/* handle mining */
		if (mineAnimation.getGoing()) {
			if (mineAnimation.update(time)) {
				toolUsing = NO_TOOL;
			}

		} else if (mineCooldown.getGoing()) {
			if (mineCooldown.update(time)) {
				toolUsing = NO_TOOL;
			}
		}

		auto startPosition = Vector(x, y);

		auto holdRight = false;
		auto holdLeft = false;
		auto holdUp = false;
		auto holdDown = false;

		auto push = Vector(0, 0);

		/* right */
		if (input->getKeyDown(GLFW_KEY_D)) {
			push.addX(WALK_SPEED);
			holdRight = true;
			facing = RIGHT;
		}

		/* left */
		if (input->getKeyDown(GLFW_KEY_A)) {
			push.addX(-WALK_SPEED);

			if (holdRight) {
				holdRight = false;
				holdLeft = false;
			} else {
				holdLeft = true;
				facing = LEFT;
			}
		}

		if (input->getKeyPressed(GLFW_KEY_W)) {
			if (
				room->getBlock(x - room->x, y - 0.25_f32 - room->y).getType() != Block::AIR ||
				wallSiding != Point::DOWN
			) {
				velocityY = JUMP_SPEED;
				wallSiding = Point::DOWN;
			}
		}

		auto centerX = x, centerY = y + 0.5_f32;

		if (!mineAnimation.getGoing() && !mineCooldown.getGoing()) {
			if (input->getKeyPressed(GLFW_KEY_I)) {
				toolUsing = Block::DIRT;
			} else if (input->getKeyPressed(GLFW_KEY_O)) {
				toolUsing = Block::STONE;
			} else if (input->getKeyPressed(GLFW_KEY_P)) {
				toolUsing = Block::CRYSTAL;
			}

			if (toolUsing != NO_TOOL) {
				if (input->getKeyDown(GLFW_KEY_S)) {
					swingDirection = Point::DOWN;
				} else if (input->getKeyDown(GLFW_KEY_W)) {
					swingDirection = Point::UP;
				} else if (facing == RIGHT) {
					swingDirection = Point::RIGHT;
				} else {
					swingDirection = Point::LEFT;
				}

				auto direction = Point(swingDirection);

				/* the reach of the player's swing */
				auto startX = blockPos(centerX - room->x);
				auto endX = blockPos(centerX + direction.x - room->x);
				auto startY = blockPos(centerY - room->y);
				auto endY = blockPos(centerY + direction.y - room->y);

				if (swingDirection == Point::UP) {
					for (auto by = startY; by <= endY; ++by)
						if (attemptMineBlock(room->getBlock(startX, by), by == endY)) break;

				} else if (swingDirection == Point::RIGHT) {
					for (auto bx = startX; bx <= endX; ++bx)
						if (attemptMineBlock(room->getBlock(bx, startY), bx == endX)) break;

				} else if (swingDirection == Point::DOWN) {
					for (auto by = startY; by >= endY; --by)
						if (attemptMineBlock(room->getBlock(startX, by), by == endY)) break;

				} else if (swingDirection == Point::LEFT) {
					for (auto bx = startX; bx >= endX; --bx)
						if (attemptMineBlock(room->getBlock(bx, startY), bx == endX)) break;
				}
			}
		}

		velocityX += push.getX() * time;

		auto leftWall = room->getBlock(centerX - 0.25_f32 - room->x, centerY - room->y);
		auto rightWall = room->getBlock(centerX + 0.25_f32 - room->x, centerY - room->y);

		/* check wall slide status */
		if (wallSiding == Point::LEFT) {
			if (leftWall.getType() == Block::AIR) {
				wallSiding = Point::DOWN;
			}
		} else if (wallSiding == Point::RIGHT) {
			if (rightWall.getType() == Block::AIR) {
				wallSiding = Point::DOWN;
			}
		} else {
			if (holdRight && rightWall.getType() != Block::AIR) {
				wallSiding = Point::RIGHT;
			} else if (holdLeft && leftWall.getType() != Block::AIR) {
				wallSiding = Point::LEFT;
			}
		}

		if (y > room->x + room->height) wallSiding = Point::DOWN;

		/* decay speed */
		if (!holdRight && velocityX > 0) {
			velocityX -= WALK_SPEED * time;
			if (velocityX < 0) velocityX = 0;
		}

		if (!holdLeft && velocityX < 0) {
			velocityX += WALK_SPEED * time;
			if (velocityX > 0) velocityX = 0;
		}

		/* gravity and vertical speed limiter */
		if (wallSiding == Point::DOWN) {
			velocityY += GRAVITY * time;

			if (velocityY < MIN_FALL_SPEED) velocityY = MIN_FALL_SPEED;

		} else {
			/* don't fall as fast when wall sliding */
			if (velocityY < 0)
				velocityY += GRAVITY * time / 4_f32;
			else
				velocityY += GRAVITY * time;

			if (velocityY < MIN_SLIDE_SPEED) {
				velocityY += WALK_SPEED * time;
				if (velocityY > MIN_SLIDE_SPEED) velocityY = MIN_SLIDE_SPEED;
			}
		}

		/* limit walk speed */
		if (velocityX > MAX_WALK_SPEED) velocityX = MAX_WALK_SPEED;
		if (velocityX < -MAX_WALK_SPEED) velocityX = -MAX_WALK_SPEED;

		auto endPosition = startPosition + Vector(velocityX * time, velocityY * time);

		i32 whoCollided;
		auto collisionX = 0.0_f32, collisionY = 0.0_f32;

		/* apply a series of collisions, modifying the endPosition */
		/* until no collisions are found */
		do {
			whoCollided = WHO_NONE;

			auto nearestAlong = 100000.0_f32;

			auto movingLeft = endPosition.getX() < startPosition.getX();
			auto movingRight = endPosition.getX() > startPosition.getX();
			auto movingDown = endPosition.getY() < startPosition.getY();
			auto movingUp = endPosition.getY() > startPosition.getY();

			auto left = movingLeft ? blockPos(endPosition.getX()) : positiveBlockPos(startPosition.getX());
			auto right = movingLeft ? blockPos(startPosition.getX()) : blockPos(endPosition.getX());
			auto down = movingDown ? blockPos(endPosition.getY()) : positiveBlockPos(startPosition.getY());
			auto up = movingDown ? blockPos(startPosition.getY()) : blockPos(endPosition.getY());

			if (movingLeft) {
				for (auto blockX = right - 1; blockX >= left; --blockX) {
					collisionX = blockX + 1_f32;

					auto [collision, along] = collisionBlock(room, COLL_X, blockX, collisionX, startPosition, endPosition);

					if (collision && along < nearestAlong && collisionX != endPosition.getX()) {
						nearestAlong = along;
						collisionX += FLOAT_SKEK;
						whoCollided = WHO_X;
						break;
					}
				}
			} else if (movingRight) {
				for (auto blockX = left + 1; blockX <= right; ++blockX) {
					collisionX = (f32) blockX;

					auto [collision, along] = collisionBlock(room, COLL_X, blockX, collisionX, startPosition, endPosition);

					if (collision && along < nearestAlong && collisionX != endPosition.getX()) {
						nearestAlong = along;
						collisionX -= FLOAT_SKEK;
						whoCollided = WHO_X;
						break;
					}
				}
			}

			if (movingDown) {
				for (auto blockY = up - 1; blockY >= down; --blockY) {
					collisionY = blockY + 1_f32;

					auto [collision, along] = collisionBlock(room, COLL_Y, blockY, collisionY, startPosition, endPosition);

					if (collision && along < nearestAlong && collisionY != endPosition.getY()) {
						nearestAlong = along;
						collisionY += FLOAT_SKEK;
						whoCollided = WHO_Y;
						break;
					}
				}
			} else if (movingUp) {
				for (auto blockY = down + 1; blockY <= up; ++blockY) {
					collisionY = (f32) blockY;

					auto [collision, along] = collisionBlock(room, COLL_Y, blockY, collisionY, startPosition, endPosition);

					if (collision && along < nearestAlong && collisionY != endPosition.getY()) {
						nearestAlong = along;
						collisionY -= FLOAT_SKEK;
						whoCollided = WHO_Y;
						break;
					}
				}
			}

			if (whoCollided == WHO_X) {
				endPosition.setX(collisionX);
				velocityX = 0;

			} else if (whoCollided == WHO_Y) {
				endPosition.setY(collisionY);
				velocityY = 0;
			}
		} while (whoCollided != WHO_NONE);

		x = endPosition.getX();
		y = endPosition.getY();
	}

	auto Player::render(CNGE::Camera & camera, TextureShader & textureShader, CNGE::TileGrid * playerTileGrid, Rect & rect) -> void {
		textureShader.enable(CNGE::Transform::toModel(
			x - 0.5_f32, y, 1_f32, 1_f32
		), camera.getProjview());
		textureShader.giveParams(playerTileGrid->getSheet(0));

		playerTileGrid->bind();

		rect.render();
	}

	auto Player::renderTool(CNGE::Camera & camera, TextureShader & textureShader, CNGE::TileGrid * toolTileGrid, Rect & rect) -> void {
		f32 startAngle, direction;

		if (swingDirection == Point::UP) {
			startAngle = CNGE::PI<f32> * 3_f32 / 4_f32;
			direction = -1_f32;
		} else if (swingDirection == Point::RIGHT) {
			startAngle = CNGE::PI<f32> / 4_f32;
			direction = -1_f32;
		} else if (swingDirection == Point::DOWN) {
			startAngle = CNGE::PI<f32> * 7_f32 / 4_f32;
			direction = -1_f32;
		} else /* left */ {
			startAngle = CNGE::PI<f32> * 3_f32 / 4_f32;
			direction = 1_f32;
		}

		/* correct for the tool image being angled */
		startAngle -= CNGE::PI<f32> / 4_f32;

		bool shouldRender = false;
		f32 angle;

		if (mineAnimation.getGoing()) {
			/* full swing in direction + 45 degrees */
			auto endAngle = startAngle + (CNGE::PI<f32> / 2_f32 * direction);

			auto along = mineAnimation.getAlong();

			angle = CNGE::interpReverseSquared(startAngle, endAngle, along);

			shouldRender = true;

		} else if (mineCooldown.getGoing()) {
			auto midAngle = startAngle + (CNGE::PI<f32> / 4_f32 * direction);
			auto finalAngle = startAngle - (CNGE::PI<f32> / 2_f32 * direction);

			auto along = mineCooldown.getAlong();

			/* half swing down */
			if (along < 0.2) {
				angle = CNGE::interp(startAngle, midAngle, along * 5_f32);

			/* full swing back in opposite direction */
			} else {
				angle = CNGE::interpReverseSquared(midAngle, finalAngle, (along - 0.2_f32) * 1.25_f32);
			}

			shouldRender = true;
		}

		if (shouldRender) {
			textureShader.enable(CNGE::Transform::toModel(
				x, y + 0.5_f32, 1_f32, 1_f32, angle
			), camera.getProjview());
			textureShader.giveParams(toolTileGrid->getSheet(toolUsing - Block::DIRT));

			toolTileGrid->bind();

			rect.render();
		}
	}
}
