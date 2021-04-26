
#include "tileGrid.h"

namespace CNGE {
	TileGrid::TileGrid(u32 width, u32 height, u8* pixels, TextureParams params, u32 tilesWide, u32 tilesTall, u32 gap)
	: Texture(width, height, pixels, params), tilesWide(tilesWide), tilesTall(tilesTall), gap(gap) {}

	TileGrid::TileGrid(u32 width, u32 height, u8* pixels, TextureParams params, u32 tilesWide, u32 gap)
	: Texture(width, height, pixels, params), tilesWide(tilesWide), tilesTall(1), gap(gap) {}

	/*
	 * [0] width
	 * [1] height
	 * [2] x
	 * [3] y
	 */

	f32* TileGrid::getSheet(u32 x, u32 y) {
		/* get the width in pixels of the image minus each internal gap, then divide by tiles to get tile size */
		auto tileWidth = (width - (gap * (tilesWide - 1))) / tilesWide;
		auto tileHeight = (height - (gap * (tilesTall - 1))) / tilesTall;

		tileValues[0] = f32(tileWidth) / width;
		tileValues[1] = f32(tileHeight) / height;
		tileValues[2] = x * f32(tileWidth + gap) / width;
		tileValues[3] = y * f32(tileHeight + gap) / height;

		return tileValues;
	}

	f32* TileGrid::getSheet(u32 x) {
		auto tileWidth = (width - (gap * (tilesWide - 1))) / tilesWide;

		tileValues[0] = f32(tileWidth) / width;
		tileValues[1] = 1;
		tileValues[2] = x * f32(tileWidth + gap) / width;
		tileValues[3] = 0;

		return tileValues;
	}

	u32 TileGrid::getTilesWide() {
		return tilesWide;
	}

	u32 TileGrid::getTilesTall() {
		return tilesTall;
	}
}
