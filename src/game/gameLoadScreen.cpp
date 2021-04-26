
#include "gameLoadScreen.h"
#include "gameResources.h"

namespace Game {
	const CNGE::Color GameLoadScreen::backgroundColor = CNGE::Color(234, 56, 100);
	const CNGE::Color GameLoadScreen::mainColor = backgroundColor.invert();

	GameLoadScreen::GameLoadScreen() 
		: aspect(16_f32, 8_f32, 21.5_f32, 9_f32), camera() {}

	auto GameLoadScreen::resized(i32 width, i32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto GameLoadScreen::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		camera.update();
	}

	auto GameLoadScreen::render(i32 completed, i32 total) -> void {
		auto along = completed / (f32)total;

		auto screenWidth = aspect.getGameWidth();
		auto screenHeight = aspect.getGameHeight();

		auto barWidth = 10_f32;
		auto barHeight = 3_f32;

		auto barX = screenWidth / 2 - barWidth / 2;
		auto barY = screenHeight / 2 - barHeight / 2;

		GameResources::colorShader.enable();
		GameResources::colorShader.giveColor(backgroundColor);
		GameResources::rect.render();

		GameResources::colorShader.enable(CNGE::Transform::toModel(barX, barY, barWidth * along, barHeight), camera.getProjview());
		GameResources::colorShader.giveColor(mainColor);
		GameResources::rect.render();
	}
}
