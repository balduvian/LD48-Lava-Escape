
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include "game/graphics/fontTextureShader.h"
#include "game/graphics/textureShader.h"
#include "cnge/engine/entities/texture/tileGrid.h"
#include "cnge/engine/entities/texture/imageTexture.h"
#include "cnge/engine/entities/sound/sound.h"
#include "cnge/engine/entities/font/font.h"
#include "cnge/load/resource.h"

#include "graphics/rect.h"
#include "graphics/colorShader.h"
#include "graphics/textureShader.h"
#include "graphics/sdfShader.h"
#include "graphics/triangleShader.h"

namespace Game {
	class GameResources {
	public:
		static Rect rect;
		static TextureShader textureShader;
		static FontTextureShader fontTextureShader;
		static ColorShader colorShader;
		static SDFShader sdfShader;
		static TriangleShader triangleShader;
		static CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> playerTexture;
		static CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> toolTexture;
		static CNGE::Font testFont;

		static CNGE::Sound break0;
		static CNGE::Sound break1;
		static CNGE::Sound break2;
		static CNGE::Sound break3;

		static CNGE::Sound ting0;
		static CNGE::Sound ting1;
		static CNGE::Sound ting2;
		static CNGE::Sound ting3;

		static std::vector<CNLL::Resource*> loadScreenResources;
		static std::vector<CNLL::Resource*> gameResources;
	};
}

#endif
