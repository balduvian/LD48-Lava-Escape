
#include "cnge/engine/entities/texture/texture.h"
#include "textureShader.h"

namespace Game {
	TextureShader::TextureShader() : Shader("res/shaders/texture/vert.glsl", "res/shaders/texture/frag.glsl") {}

	auto TextureShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
		texModifLoc = getUniform("texModif");
	}

	auto TextureShader::giveParams(f32 r, f32 g, f32 b, f32 a, const f32 texModif[]) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, texModif);
	}

	auto TextureShader::giveParams(f32 r, f32 g, f32 b, f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}

	auto TextureShader::giveParams(const f32 texModif[]) -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, texModif);
	}

	auto TextureShader::giveParams() -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}
}
