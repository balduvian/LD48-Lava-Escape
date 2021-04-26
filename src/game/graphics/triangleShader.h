
#ifndef GAME_TRIANGLE_SHADER
#define GAME_TRIANGLE_SHADER

#include "cnge/engine/entities/shader/shader.h"
#include "cnge/types.h"
#include "cnge/util/color.h"

namespace Game {
	class TriangleShader : public CNGE::Shader {
	private:
		i32 colorLoc;
		i32 offsetLoc;

	public:
		static f32 offset[8];

		TriangleShader();

		auto getUniforms() -> void override;

		auto giveColor(const f32, const f32, const f32, const f32) -> void;

		auto giveColor(const CNGE::Color &color) -> void;

		auto giveOffset(f32 *) -> void;
	};
}

#endif