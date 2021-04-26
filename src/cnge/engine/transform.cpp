
#include "transform.h"

namespace CNGE {
	Matrix4f Transform::matrix{};

	float Transform::defaultModel[16] {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float Transform::defaultProjview[16] {
		 2,  0, 0, 0,
		 0,  2, 0, 0,
		 0,  0, 1, 0,
		-1, -1, 0, 1
	};

	Transform::Transform()
		: x(0), y(0), scaleX(1), scaleY(1), rotation(0) {}

	Transform::Transform(f32 x, f32 y, f32 scaleX, f32 scaleY, f32 rotation)
		: x(x), y(y), scaleX(scaleX), scaleY(scaleY), rotation(rotation) {}

	/* modifying a matrix */

	auto Transform::transform(Matrix4f& mat, f32 x, f32 y, f32 width, f32 height) -> float* {
		return mat.setTranslate2D(x, y).scale2D(width, height).m;
	}

	auto Transform::transform(Matrix4f& mat, f32 x, f32 y, f32 width, f32 height, f32 rotation) -> float* {
		return mat.setTranslate2D(x, y).rotate2D(rotation).scale2D(width, height).m;
	}

	auto Transform::transformCenterRotate(Matrix4f& mat, f32 x, f32 y, f32 width, f32 height, f32 rotation) -> float * {
		return mat.setTranslate2D(x + width / 2, y + height / 2).rotate2D(rotation).translate2D(-width / 2, -height / 2).scale2D(width, height).m;
	}

	auto Transform::transform(Matrix4f& mat, f32 x, f32 y, f32 z, f32 width, f32 height, f32 rotation) -> float* {
		return mat.setTranslate(x, y, z).rotate2D(rotation).scale2D(width, height).m;
	}

	auto Transform::transform(Matrix4f& mat) -> float* {
		return transform(mat, x, y, scaleX, scaleY, rotation);
	}

	auto Transform::cameraTransform(Matrix4f& mat) -> float* {
		return mat.setScale2D(scaleX, scaleY).rotate2D(-rotation).translate2D(-x, -y).m;
	}

	auto Transform::toModel() -> float* {
		return transform(matrix);
	}

	auto Transform::toModel(f32 z) -> float* {
		return transform(matrix, x, y, z, scaleX, scaleY, rotation);
	}

	auto Transform::toModel(f32 x, f32 y, f32 width, f32 height) -> float* {
		return transform(matrix, x, y, width, height);
	}

	auto Transform::toModel(f32 x, f32 y, f32 width, f32 height, f32 rotation) -> float* {
		return transform(matrix, x, y, width, height, rotation);
	}

	auto Transform::toModelCenterRotate(f32 x, f32 y, f32 width, f32 height, f32 rotation) -> float* {
		return transformCenterRotate(matrix, x, y, width, height, rotation);
	}

	auto Transform::toModel(f32 x, f32 y, f32 z, f32 width, f32 height, f32 rotation) -> float* {
		return transform(matrix, x, y, z, width, height, rotation);
	}
}
