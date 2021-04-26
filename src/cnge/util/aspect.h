
#ifndef CNGE_ASPECT
#define CNGE_ASPECT

#include "../types.h"

namespace CNGE {
	class Aspect {
	private:
		/// values we use to calculate
		f32 aspWidth, aspHeight;
		f32 maxWidth, maxHeight;

		/// calculated values we want
		f32 gameWidth, gameHeight;
		u32 portX, portY, portW, portH;

		/// helper readability functions
		f32 calcWidth(f32 aspect, f32 height);
		f32 calcHeight(f32 aspect, f32 width);

	public:
		Aspect(f32 width, f32 height);
		Aspect(f32 width, f32 height, f32 maxWidth, f32 maxHeight);

		Aspect(Aspect&&) = delete;

		void changeAspect(f32 width, f32 height);
		void changeMaxAspect(f32 width, f32 height);

		auto updateFixed(u32, u32) -> void;
		void update(u32 screenWidth, u32 screenHeight);

		f32 getGameWidth();
		f32 getGameHeight();

		u32 getLeft();
		u32 getTop();
		u32 getWidth();
		u32 getHeight();

	};

}

#endif
