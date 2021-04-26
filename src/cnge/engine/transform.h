
#pragma once

#include "../math/matrix4.h"
#include "../math/vector2.h"

namespace CNGE {
	class Transform {
	public:
		/*
		 * initializers for the default model and projection value arrays
		 */
		static float defaultModel[16];
		static float defaultProjview[16];

		f32 x, y, scaleX, scaleY, rotation;

		Transform();
		Transform(f32, f32, f32, f32, f32);

		static auto transform(Matrix4f&, f32, f32, f32, f32) -> float*;
		static auto transform(Matrix4f&, f32, f32, f32, f32, f32) -> float*;
		static auto transformCenterRotate(Matrix4f&, f32, f32, f32, f32, f32) -> float*;
		static auto transform(Matrix4f&, f32, f32, f32, f32, f32, f32) -> float*;
		auto transform(Matrix4f&) -> float*;
		auto cameraTransform(Matrix4f&) -> float*;

		auto toModel() -> float*;
		auto toModel(f32 z) -> float*;
		static auto toModel(f32, f32, f32, f32) -> float*;
		static auto toModel(f32, f32, f32, f32, f32) -> float*;
		static auto toModelCenterRotate(f32, f32, f32, f32, f32) -> float*;
		static auto toModel(f32, f32, f32, f32, f32, f32) -> float*;

	private:
		static Matrix4f matrix;
	};
}
