
#ifndef GAME_RECT
#define GAME_RECT

#include "cnge/engine/entities/vao/vao.h"

namespace Game {
	/// a texture enabled rect with
	/// a 0,0 in the bottom left corner
	class Rect : public CNGE::VAO {
	public:
		Rect();
	};

	/// a texture enabled rect with 
	/// a 0,0 in the bottom center
	class CardRect : public CNGE::VAO {
	public:
		CardRect();
	};

}

#endif
