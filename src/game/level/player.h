//
// Created by Emmet on 4/24/2021.
//

#ifndef CNGE_8_2_PLAYER_H
#define CNGE_8_2_PLAYER_H

#include <random>

#include "cnge/engine/camera.h"
#include "cnge/util/timer.h"
#include "cnge/engine/input.h"
#include "cnge/engine/entities/texture/tileGrid.h"

#include "../graphics/textureShader.h"
#include "../graphics/rect.h"
#include "room.h"

namespace Game {
	class Player {
	public:
		Player(f32, f32);

		f32 x, y;
		f32 velocityX, velocityY;
		bool facing;

		CNGE::Timer walkAnimation;
		u32 walkFrame;

		i32 toolUsing;
		i32 swingDirection;
		CNGE::Timer mineAnimation;
		CNGE::Timer mineCooldown;

		i32 wallSiding;

		std::default_random_engine random;

		constexpr static auto LEFT = false;
		constexpr static auto RIGHT = true;

		constexpr static auto JUMP_SPEED = 10.5_f32;
		constexpr static auto WALK_SPEED = 32_f32;
		constexpr static auto MAX_WALK_SPEED = 8_f32;

		constexpr static auto GRAVITY = -24_f32;
		constexpr static auto MIN_FALL_SPEED = -64_f32;
		constexpr static auto MIN_SLIDE_SPEED = -4_f32;

		constexpr static auto WIDTH = 0.5_f32;
		constexpr static auto OFF_X = -WIDTH / 2;

		constexpr static auto NO_TOOL = -1;

		auto update(Room * room, CNGE::Input *, f64) -> void;

		auto render(CNGE::Camera & camera, TextureShader & textureShader, CNGE::TileGrid * playerTileGrid, Rect & rect) -> void;

		auto renderTool(CNGE::Camera & camera, TextureShader & textureShader, CNGE::TileGrid * toolTileGrid, Rect & rect) -> void;

	private:
		auto attemptMineBlock(Block &, bool) -> bool;
	};
}

#endif //CNGE_8_2_PLAYER_H
