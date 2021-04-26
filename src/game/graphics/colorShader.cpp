#include <iostream>

#include "colorShader.h"

namespace Game {
	ColorShader::ColorShader() : Shader("res/shaders/color/vert.glsl", "res/shaders/color/frag.glsl") {};

	auto ColorShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
	}

	auto ColorShader::giveColor(const f32 r, const f32 g, const f32 b, const f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
	}
	
	auto ColorShader::giveColor(const CNGE::Color &color) -> void {
		giveVector4(colorLoc, color.r, color.g, color.b, color.a);
	}
}
