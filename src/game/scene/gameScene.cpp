
#include "cnge/math/math.h"

#include "../gameResources.h"
#include "gameScene.h"

#include <string>
#include <iostream>

namespace Game {
	GameScene::GameScene() : Scene(&GameResources::gameResources),
		camera(),
		aspect(viewWidth, viewHeight, viewWidth, viewHeight),
		currentRoom(nullptr),
		nextRoom(nullptr),
		player(nullptr),
		roomTransition(1.0),
		roomWidth(50),
		lavaSection(0),
        lavaStartTimer(2.0),
		lavaSectionTimer(7.0),
        deathTimer(4.0),
        blinkTimer(1.5),
        roomNo(0),
        pregame(false),
        lavaTime(0)
	{}

	auto GameScene::start() -> void {
		generateNextRoom();
		scaleCamera();
	}

	auto GameScene::resized(u32 width, u32 height) -> void {
		aspect.updateFixed(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto isLeft(Vector start, Vector end, f32 x, f32 y) -> bool {
		auto val = (end.getX() - start.getX()) * (y - start.getY()) - (end.getY() - start.getY()) * (x - start.getX());
		return val > 0;
	}

	auto GameScene::update(CNGE::Input * input, CNGE::Timing * timing) -> void {
		/* scrolling to next room */
		if (roomTransition.getGoing()) {
			f32 cameraX, cameraY;

			if (roomTransition.update(timing->time)) {
				cameraX = nextRoom->x;
				cameraY = nextRoom->y;

				currentRoom = std::move(nextRoom);
				nextRoom = nullptr;

				startRoom();

			} else {
				auto along = roomTransition.getAlong();

				cameraX = CNGE::interp((f32) currentRoom->x, (f32) nextRoom->x, along);
				cameraY = CNGE::interp((f32) currentRoom->y, (f32) nextRoom->y, along);
			}

			camera.transform.x = cameraX;
			camera.transform.y = cameraY;

			/* playing in the room */
		} else {
			if (pregame) {
				if (
					input->getKeyDown(GLFW_KEY_I) ||
					input->getKeyDown(GLFW_KEY_O) ||
					input->getKeyDown(GLFW_KEY_P)
				) {
					pregame = false;
					lavaStartTimer.start();
				}

				blinkTimer.updateContinual(timing->time);
			}

			if (player != nullptr) player->update(currentRoom.get(), input, timing->time);

			if (lavaStartTimer.getGoing()) {
				if (lavaStartTimer.update(timing->time)) lavaSectionTimer.start();

			} else if (lavaSectionTimer.getGoing()) {
				/* lava engulfs screen effect when dead */
				auto lavaTime = timing->time;
				if (deathTimer.getGoing()) lavaTime *= 3;

				if (lavaSectionTimer.updateContinual(lavaTime)) {
					++lavaSection;

					if (lavaSection == currentRoom->sections.size()) lavaSectionTimer.stop();
				}
			}

			if (player != nullptr) {
				/* next room condition */
				auto exitOrdinal = currentRoom->exitDirection.ordinal();

				if (exitOrdinal == Point::LEFT) {
					if (player->x < currentRoom->x) generateNextRoom();
				} else if (exitOrdinal == Point::RIGHT) {
					if (player->x > currentRoom->width + currentRoom->x) generateNextRoom();
				} else if (exitOrdinal == Point::DOWN) {
					if (player->y < currentRoom->y) generateNextRoom();
				}

				/* lava death condition */
				if (!lavaStartTimer.getGoing()) {
					for (auto i = 0; i < currentRoom->sections.size(); ++i) {
						auto &section = currentRoom->sections.at(i);

						/* is the player in this section */
						if (
							player->x >= (i32) section.x + (i32) currentRoom->x &&
							player->y >= (i32) section.y + (i32) currentRoom->y &&
							player->x <= (i32) section.x + (i32) section.width + (i32) currentRoom->x &&
							player->y <= (i32) section.y + (i32) section.height + (i32) currentRoom->y
						) {
							auto along = lavaSectionTimer.getAlong();

							if (i == lavaSection) {
								auto currentLinePair = section.currentPair(along);
								auto roomAdd = Vector(currentRoom->x, currentRoom->y);

								/* check previous pairs in this section */
								for (auto i = 0; i < currentLinePair; ++i) {
									auto[left, right] = section.linePairs[i].getPoints(1.0_f32);

									if (isLeft(right + roomAdd, left + roomAdd, player->x, player->y)) {
										player = nullptr;
										deathTimer.start();
									}
								}

								/* check current pair in this section */
								if (player != nullptr) {
									auto[left, right] = section.linePairs[currentLinePair].getPoints(
										section.currentAlong(along));

									if (isLeft(right + roomAdd, left + roomAdd, player->x, player->y)) {
										player = nullptr;
										deathTimer.start();
									}
								}

							} else if (i < lavaSection) {
								player = nullptr;
								deathTimer.start();
							}

							break;
						}
					}
				}
			}

			if (deathTimer.update(timing->time)) {
				currentRoom = nullptr;
				generateNextRoom();
			}
		}

		camera.update();
	}

	auto GameScene::render() -> void {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		if (nextRoom != nullptr) renderRoom(nextRoom.get(), LAYER_BACKGROUND);
		renderRoom(currentRoom.get(), LAYER_BACKGROUND);

		if (player != nullptr) player->render(camera, GameResources::textureShader, GameResources::playerTexture.get(), GameResources::rect);

		if (player != nullptr) player->renderTool(camera, GameResources::textureShader, GameResources::toolTexture.get(), GameResources::rect);

		renderRoom(currentRoom.get(), LAYER_LAVA);

		renderRoom(currentRoom.get(), LAYER_BEDROCK);
		if (nextRoom != nullptr) renderRoom(nextRoom.get(), LAYER_BEDROCK);

		/* pregame reminder */
		if (pregame && blinkTimer.getAlong() < 0.5) {
			GameResources::testFont.getTexture()->bind();

			GameResources::testFont.render((f32)viewWidth / 2_f32 - 128, 96, 32, "Press I, O, or P to start",
			[this] (f32 x, f32 y, f32 width, f32 height, f32 * tile) {
			   GameResources::fontTextureShader.enable(CNGE::Transform::toModel(x, y, width, height), camera.getProjection());
			   GameResources::fontTextureShader.giveParams(tile);
			   GameResources::rect.render();
			});
		}

		/* room number indicator */
		GameResources::testFont.getTexture()->bind();

		GameResources::testFont.render((f32)viewWidth - 32_f32, 32_f32, 32, std::to_string(roomNo).c_str(),
		[this] (f32 x, f32 y, f32 width, f32 height, f32 * tile) {
			GameResources::fontTextureShader.enable(CNGE::Transform::toModel(x, y, width, height), camera.getProjection());
			GameResources::fontTextureShader.giveParams(tile);
			GameResources::rect.render();
		});

		/* red fade out */
		if (deathTimer.getGoing()) {
			auto along = deathTimer.getAlong();

			GameResources::colorShader.enable();
			GameResources::colorShader.giveColor(0.9, 0, 0, along);
			GameResources::rect.render();
		}
	}

	auto GameScene::switchScene() -> SceneSwitchReturn {
		return dontSwitch();
	}

	/* ------------------------------------------------- */

	auto GameScene::roomHeight() -> u32 {
		return (roomWidth * viewHeight) / viewWidth;
	}

	auto GameScene::generateNextRoom() -> void {
		/* the first room generated */
		if (currentRoom == nullptr) {
			pregame = true;
			blinkTimer.start();

			currentRoom = std::make_unique<Room>(0, 0, roomWidth, roomHeight(), 4, 2, 2);
			currentRoom->generate(nullptr, { 0, -1 });

			spawnInPlayer();

			camera.transform.x = 0;
			camera.transform.y = 0;

			roomNo = 0;

			lavaTime = STARTING_LAVA_TIME;

			startRoom();

		/* subsequent rooms */
		} else {
			auto direction = currentRoom->exitDirection;

			nextRoom = std::make_unique<Room>(
				currentRoom->x + direction.x * roomWidth,
				currentRoom->y + direction.y * roomHeight(),
				roomWidth, roomHeight(), 4, 2, 2
			);

			nextRoom->generate(currentRoom->lastSection(), currentRoom->exitDirection);

			++roomNo;

			lavaTime -= (f32)currentRoom->sections.size() / 16_f32;
			if (lavaTime < MIN_LAVA_TIME) lavaTime = MIN_LAVA_TIME;

			roomTransition.start();
		}
	}

	auto GameScene::startRoom() -> void {
		lavaSection = 0;
		if (!pregame) lavaStartTimer.start();

		lavaSectionTimer.setTime(lavaTime);
		lavaSectionTimer.stop();
	}

	auto GameScene::spawnInPlayer() -> void {
		player = std::make_unique<Player>(currentRoom->playerSpawn.x + 0.5_f32 + currentRoom->x, currentRoom->playerSpawn.y + currentRoom->y);
	}

	auto GameScene::renderRoom(Room * room, i32 layer) -> void {
		if (layer == LAYER_BACKGROUND) {
			GameResources::colorShader.enable(CNGE::Transform::toModel(
				(f32)room->x, (f32)room->y, (f32)room->width, (f32)room->height
			), camera.getProjview());
			GameResources::colorShader.giveColor(0.1_f32, 0.1_f32, 0.1_f32, 1.0_f32);
			GameResources::rect.render();

			renderBlocks(room, false);

		} else if (layer == LAYER_LAVA) {
			renderLava(room, lavaSection, lavaSectionTimer.getAlong());

		} else if (layer == LAYER_BEDROCK) {
			renderBlocks(room, true);
		}
	}

	auto GameScene::renderBlocks(Room * room, bool bedrock) -> void {
		for (auto x = 0; x < room->width; ++x) {
			for (auto y = 0; y < room->height; ++y) {
				auto & block = room->blocks[y * room->width + x];

				if (bedrock) {
					if (block.getType() == Block::BEDROCK) {
						GameResources::colorShader.giveColor(0.2_f32, 0.2_f32, 0.2_f32, 1.0_f32);
					} else {
						continue;
					}
				} else {
					if (block.getType() == Block::DIRT) {
						GameResources::colorShader.giveColor(0.6_f32, 0.3_f32, 0.2_f32, 1.0_f32);
					} else if (block.getType() == Block::STONE) {
						GameResources::colorShader.giveColor(0.5_f32, 0.5_f32, 0.5_f32, 1.0_f32);
					} else if (block.getType() == Block::CRYSTAL) {
						GameResources::colorShader.giveColor(0.1_f32, 0.7_f32, 0.8_f32, 1.0_f32);
					} else {
						continue;
					}
				}

				GameResources::colorShader.enable(CNGE::Transform::toModel(
					x + room->x, y + room->y, 1, 1
				), camera.getProjview());

				GameResources::rect.render();
			}
		}
	}

	auto renderLavaLinePair(Room * room, LinePair & linePair, f32 along) {
		/* assume shader already enabled and given color */
		auto [left, right] = linePair.getPoints(along);

		TriangleShader::offset[0] = linePair.startRight.x + room->x;
		TriangleShader::offset[1] = linePair.startRight.y + room->y;
		TriangleShader::offset[2] = linePair.startLeft.x + room->x;
		TriangleShader::offset[3] = linePair.startLeft.y + room->y;
		TriangleShader::offset[4] = left.getX() + room->x;
		TriangleShader::offset[5] = left.getY() + room->y;
		TriangleShader::offset[6] = right.getX() + room->x;
		TriangleShader::offset[7] = right.getY() + room->y;

		GameResources::triangleShader.giveOffset(TriangleShader::offset);
		GameResources::rect.render();
	}

	auto GameScene::renderLava(Room * room, i32 renderLavaSection, f32 along) -> void {
		GameResources::triangleShader.enable(CNGE::Transform::defaultModel, camera.getProjview());
		GameResources::triangleShader.giveColor(1.0, 0.25, 0.0, 1.0);

		/* render complete past lava sections */
		for (auto i = 0; i < renderLavaSection; ++i) {
			for (auto & linePair : room->sections[i].linePairs) {
				renderLavaLinePair(room, linePair, 1.0_f32);
			}
		}

		if (renderLavaSection < room->sections.size()) {
			auto &section = room->sections.at(renderLavaSection);

			auto linePairIndex = section.currentPair(along);

			/* complete lava sections before this in this section */
			for (auto i = 0; i < linePairIndex; ++i) {
				renderLavaLinePair(room, section.linePairs.at(i), 1.0_f32);
			}

			renderLavaLinePair(room, section.linePairs.at(linePairIndex), section.currentAlong(along));
		}
	}

	auto GameScene::scaleCamera() -> void {
		camera.transform.scaleX = (f32)viewWidth / (f32)roomWidth;
		camera.transform.scaleY = (f32)viewHeight / (f32)roomHeight();
	}

	auto GameScene::renderLine(f32 x, f32 y, Point start, Point end) -> void {
		auto startVector = Vector(start.x + x, start.y + y);
		auto endVector = Vector(end.x + x, end.y + y);

		auto lineVector = endVector - startVector;
		auto midPoint = (lineVector * 0.5_f32) + startVector;

		//GameResources::colorShader.enable(CNGE::Transform::toModelCenterRotate(
		//	midPoint.getX(), midPoint.getY(),
		//	lineVector.length(), 2.0f,
		//	lineVector.angle()
		//), camera.getProjview());

		GameResources::colorShader.enable(CNGE::Transform::toModel(
			start.x + x - 0.5_f32, start.y + y - 0.5_f32, 1, 1
		), camera.getProjview());
		GameResources::colorShader.giveColor(1.0, 0.0, 0.0, 0.5);
		GameResources::rect.render();

		GameResources::colorShader.enable(CNGE::Transform::toModel(
			end.x + x - 0.5_f32, end.y + y - 0.5_f32, 1, 1
		), camera.getProjview());
		GameResources::colorShader.giveColor(0.0, 0.0, 1.0, 0.5);
		GameResources::rect.render();
	}
}

/*
		for (auto & section : room->sections) {
			GameResources::colorShader.enable(CNGE::Transform::toModel(
				(f32)section.x + (f32)room->x,
				(f32)section.y + (f32)room->y,
				(f32)section.width,
				(f32)section.height
			), camera.getProjview());
			//GameResources::colorShader.giveColor(section.indexX * 0.2_f32, 0.0_f32, section.indexY * 0.2_f32, 1.0_f32);
			GameResources::colorShader.giveColor(0.2_f32, 0.2_f32, 0.2_f32, 1.0_f32);
			GameResources::rect.render();
		}
		*/

//for (auto & linePair : room->linePairs) {
//	renderLine(room->x, room->y, linePair.startLeft, linePair.endLeft);
//	renderLine(room->x, room->y, linePair.startRight, linePair.endRight);
//}