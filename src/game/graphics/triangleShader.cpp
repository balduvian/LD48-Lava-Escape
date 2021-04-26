
#include "triangleShader.h"

namespace Game {
	TriangleShader::TriangleShader() : Shader("res/shaders/triangle/vert.glsl", "res/shaders/triangle/frag.glsl") {}

	f32 TriangleShader::offset[8] {};

	auto TriangleShader::getUniforms() -> void {
		colorLoc = getUniform("inColor");
		offsetLoc = getUniform("offset");
	}

	auto TriangleShader::giveColor(const f32 r, const f32 g, const f32 b, const f32 a) -> void {
		giveVector4(colorLoc, r, g, b, a);
	}
	
	auto TriangleShader::giveColor(const CNGE::Color &color) -> void {
		giveVector4(colorLoc, color.r, color.g, color.b, color.a);
	}

	auto TriangleShader::giveOffset(f32 * offset) -> void {
		giveVector2Array(offsetLoc, 8, offset);
	}
}
