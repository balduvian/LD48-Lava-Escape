
#include "cnge/engine/entities/texture/texture.h"
#include "sdfShader.h"

namespace Game {
	SDFShader::SDFShader() : Shader("res/shaders/sdf/vert.glsl", "res/shaders/sdf/frag.glsl") {}

	auto SDFShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
		texModifLoc = getUniform("texModif");
	}

	auto SDFShader::giveParams(f32 r, f32 g, f32 b, f32 a, const f32 texModif[]) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, texModif);
	}

	auto SDFShader::giveParams(f32 r, f32 g, f32 b, f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}

	auto SDFShader::giveParams(const f32 texModif[]) -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, texModif);
	}

	auto SDFShader::giveParams() -> void {
		giveVector4(colorLoc, 1, 1, 1, 1);
		giveVector4(texModifLoc, CNGE::Texture::DEFAULT_TILE_VALUES);
	}
}
