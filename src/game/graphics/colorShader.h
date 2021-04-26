
#ifndef GAME_COLORSHADER
#define GAME_COLORSHADER

#include "cnge/engine/entities/shader/shader.h"
#include "cnge/types.h"
#include "cnge/util/color.h"

namespace Game {
	class ColorShader : public CNGE::Shader {
	private:
		i32 colorLoc;

	public:
		ColorShader();

		auto getUniforms() -> void override;

		auto giveColor(const f32, const f32, const f32, const f32) -> void;

		auto giveColor(const CNGE::Color &color) -> void;
	};
}

#endif