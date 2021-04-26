
#include "gameResources.h"

namespace Game {
	Rect GameResources::rect = Rect();
	TextureShader GameResources::textureShader = TextureShader();
	FontTextureShader GameResources::fontTextureShader = FontTextureShader();
	ColorShader GameResources::colorShader = ColorShader();
	SDFShader GameResources::sdfShader = SDFShader();
	TriangleShader GameResources::triangleShader = TriangleShader();
	CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> GameResources::playerTexture = CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32>("res/player.png", CNGE::TextureParams().setWrap(GL_CLAMP_TO_BORDER), 3, 1, 0);
	CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> GameResources::toolTexture = CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32>("res/tools.png", CNGE::TextureParams().setWrap(GL_CLAMP_TO_BORDER), 3, 1, 0);
	CNGE::Font GameResources::testFont = CNGE::Font("res/font.fnt", CNGE::TextureParams());

	CNGE::Sound GameResources::break0 = CNGE::Sound("res/sound/break0.wav");
	CNGE::Sound GameResources::break1 = CNGE::Sound("res/sound/break1.wav");
	CNGE::Sound GameResources::break2 = CNGE::Sound("res/sound/break2.wav");
	CNGE::Sound GameResources::break3 = CNGE::Sound("res/sound/break3.wav");

	CNGE::Sound GameResources::ting0 = CNGE::Sound("res/sound/ting0.wav");
	CNGE::Sound GameResources::ting1 = CNGE::Sound("res/sound/ting1.wav");
	CNGE::Sound GameResources::ting2 = CNGE::Sound("res/sound/ting2.wav");
	CNGE::Sound GameResources::ting3 = CNGE::Sound("res/sound/ting3.wav");

	std::vector<CNLL::Resource*> GameResources::loadScreenResources = {
		&rect, &colorShader
	};

	std::vector<CNLL::Resource*> GameResources::gameResources = {
		&rect,
		&colorShader,
		&textureShader,
		&testFont,
		&fontTextureShader,
		&triangleShader,
		&playerTexture,
		&toolTexture,
		&break0,
		&break1,
		&break2,
		&break3,
		&ting0,
		&ting1,
		&ting2,
		&ting3
	};
}
