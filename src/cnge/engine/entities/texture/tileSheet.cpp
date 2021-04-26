
#include "tileSheet.h"

namespace CNGE {
	TileSheet::TileSheet(u32 width, u32 height, u8* pixels, TextureParams params, u32 numPositions, i32* positions)
		: Texture(width, height, pixels, params), numPositions(numPositions), inputPositions(positions), calculatedPositions()
	{
		calculatedPositions = new f32[numPositions * 4];

		auto widthFraction = 1._f32 / width;
		auto heightFraction = 1._f32 / height;

		for (auto i = 0; i < numPositions; ++i) {
			calculatedPositions[i * 4 + 0] =  widthFraction * inputPositions[i * 4 + 0];
			calculatedPositions[i * 4 + 1] = heightFraction * inputPositions[i * 4 + 1];
			calculatedPositions[i * 4 + 2] =  widthFraction * inputPositions[i * 4 + 2];
			calculatedPositions[i * 4 + 3] = heightFraction * inputPositions[i * 4 + 3];
		}

		delete[] inputPositions;
	}

	f32* TileSheet::getSheet(u32 tile) {
		tileValues[0] = calculatedPositions[tile * 4 + 0];
		tileValues[1] = calculatedPositions[tile * 4 + 1];
		tileValues[2] = calculatedPositions[tile * 4 + 2];
		tileValues[3] = calculatedPositions[tile * 4 + 3];

		return tileValues;
	}

	TileSheet::~TileSheet() {
		delete[] calculatedPositions;
		calculatedPositions = nullptr;
	}
}
