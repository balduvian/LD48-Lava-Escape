
#ifndef CNGE_TILE_SHEET
#define CNGE_TILE_SHEET

#include "texture.h"

#include "cnge/types.h"

namespace CNGE {
	class TileSheet : public Texture {
	private:
		u32 numPositions;
		const i32* inputPositions;

		f32* calculatedPositions;

	public:
		TileSheet(u32, u32, u8*, TextureParams, u32, i32*);

		f32* getSheet(u32);

		~TileSheet();
	};
}

#endif