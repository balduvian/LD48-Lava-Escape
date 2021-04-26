
#include "cnge/engine/entities/texture/texture.h"
#include "fontTextureShader.h"

namespace Game {
	FontTextureShader::FontTextureShader() : Shader("res/shaders/fontTexture/vert.glsl", "res/shaders/fontTexture/frag.glsl") {}

	auto FontTextureShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
		texModifLoc = getUniform("texModif");
	}

	auto FontTextureShader::giveParams(f32 r, f32 g, f32 b, f32 a, const f32 texModif[]) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, texModif);
	}

	auto FontTextureShader::giveParams(f32 r, f32 g, f32 b, f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}

	auto FontTextureShader::giveParams(const f32 texModif[]) -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, texModif);
	}

	auto FontTextureShader::giveParams() -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}
}
