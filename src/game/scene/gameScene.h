
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include <memory>
#include <random>

#include "cnge/engine/camera.h"
#include "cnge/util/aspect.h"
#include "cnge/engine/loop/timing.h"
#include "cnge/scene/scene.h"
#include "cnge/util/color.h"
#include "cnge/util/timer.h"

#include "../level/player.h"
#include "../level/room.h"

namespace Game {
	class GameScene : public CNGE::Scene {
	private:
		CNGE::Camera camera;
		CNGE::Aspect aspect;

		std::unique_ptr<Room> currentRoom;
		std::unique_ptr<Room> nextRoom;

		std::unique_ptr<Player> player;

		CNGE::Timer roomTransition;

		i32 lavaSection;
		CNGE::Timer lavaStartTimer;
		CNGE::Timer lavaSectionTimer;

		CNGE::Timer deathTimer;
		CNGE::Timer blinkTimer;

		i32 roomNo;
		bool pregame;
		f32 lavaTime;

	public:
		GameScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;

		auto switchScene() -> SceneSwitchReturn override;

		/* ------------------------------------------------ */

		constexpr static auto viewWidth = 512;
		constexpr static auto viewHeight = 256;

		constexpr static auto LAYER_BACKGROUND = 0;
		constexpr static auto LAYER_PLAYER = 1;
		constexpr static auto LAYER_PICKAXE = 3;
		constexpr static auto LAYER_LAVA = 4;
		constexpr static auto LAYER_BEDROCK = 5;

		constexpr static auto STARTING_LAVA_TIME = 12_f32;
		constexpr static auto MIN_LAVA_TIME = 4_f32;

		u32 roomWidth;
		auto roomHeight() -> u32;

		auto generateNextRoom() -> void;
		auto startRoom() -> void;
		auto spawnInPlayer() -> void;

		auto renderRoom(Room * room, i32 layer) -> void;
		auto renderBlocks(Room * room, bool) -> void;
		auto renderLine(f32 x, f32 y, Point, Point) -> void;
		auto renderLava(Room * room, i32 renderLavaSection, f32 along) -> void;

		auto scaleCamera() -> void;
	};
}

#endif
