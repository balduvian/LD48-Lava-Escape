
#include "aspect.h"

namespace CNGE {
	Aspect::Aspect(f32 width, f32 height)
	: gameWidth(), gameHeight(), portX(), portY(), portW(), portH(),
	aspWidth(width), aspHeight(height), maxWidth(width), maxHeight(height) {}

	Aspect::Aspect(f32 width, f32 height, f32 maxWidth, f32 maxHeight)
	: gameWidth(), gameHeight(), portX(), portY(), portW(), portH(),
	aspWidth(width), aspHeight(height), maxWidth(maxWidth), maxHeight(maxHeight) {}

	void Aspect::changeAspect(f32 width, f32 height) {
		aspWidth = width;
		aspHeight = height;
	}

	void Aspect::changeMaxAspect(f32 width, f32 height) {
		maxWidth = width;
		maxHeight = height;
	}

	f32 Aspect::calcWidth(f32 aspect, f32 height) {
		return aspect * height;
	}

	f32 Aspect::calcHeight(f32 aspect, f32 width) {
		return (1._f32 / aspect) * width;
	}

	auto Aspect::updateFixed(u32 screenWidth, u32 screenHeight) -> void {
		auto preferredAspect = aspWidth / aspHeight;
		auto screenAspect = (f32)screenWidth / (f32)screenHeight;

		/* maintains the game */
		gameWidth = aspWidth;
		gameHeight = aspHeight;

		/* screen is too tall */
		if (preferredAspect > screenAspect) {
			portW = screenWidth;
			portX = 0;

			portH = screenWidth * aspHeight / aspWidth;
			portY = (screenHeight - portH) / 2;

		/* screen is too wide */
		} else {
			portH = screenHeight;
			portY = 0;

			portW = screenHeight * aspWidth / aspHeight;
			portX = (screenWidth - portW) / 2;
		}
	}

	/// call this whenever the screen changes size
	/// or whenever you need to get initial game dimensions
	void Aspect::update(u32 screenWidth, u32 screenHeight) {
		// we need to know the aspect of the window
		auto prefAspect = aspWidth / aspHeight;
		auto screenAspect = (f32)screenWidth / screenHeight;

		// if the window is too tall
		if (prefAspect > screenAspect) {
			// the game is all the way across
			// the width of the window
			gameWidth = aspWidth;
			portX = 0;
			portW = screenWidth;

			// the tallest we want it to be
			auto minAspect = aspWidth / maxHeight;

			// limit our aspect
			if (screenAspect < minAspect)
				screenAspect = minAspect;
			
			// position centered vertically
			portH = calcHeight(screenAspect, portW);
			portY = (screenHeight - portH) / 2;

			// and then finally use it to calculate gameHeight
			gameHeight = calcHeight(screenAspect, gameWidth);
		}
		// if the window is long
		else {
			// the game takes up all the vertical space
			gameHeight = aspHeight;
			portY = 0;
			portH = screenHeight;

			// the widest we want it to be
			auto maxAspect = maxWidth / aspHeight;

			// limit our aspect
			if (screenAspect > maxAspect)
				screenAspect = maxAspect;

			// position centered vhorizontally
			portW = calcWidth(screenAspect, portH);
			portX = (screenWidth - portW) / 2;

			// and then finally use it to calculate gameWidth
			gameWidth = calcWidth(screenAspect, gameHeight);
		}
	}

	/*
	 * getters
	 */

	f32 Aspect::getGameWidth() {
		return gameWidth;
	}

	f32 Aspect::getGameHeight() {
		return gameHeight;
	}

	u32 Aspect::getLeft() {
		return portX;
	}

	u32 Aspect::getTop() {
		return portY;
	}

	u32 Aspect::getWidth() {
		return portW;
	}

	u32 Aspect::getHeight() {
		return portH;
	}

}
