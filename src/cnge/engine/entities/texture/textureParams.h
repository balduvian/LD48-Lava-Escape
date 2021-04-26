
#ifndef CNGE_TEXTURE_PARAMS
#define CNGE_TEXTURE_PARAMS

#include "cnge/types.h"

namespace CNGE {
	class TextureParams {
	private:
		static i32 defaultHorzWrap;
		static i32 defaultVertWrap;
		static i32 defaultMinFilter;
		static i32 defaultMagFilter;

		i32 horzWrap;
		i32 vertWrap;
		i32 minFilter;
		i32 magFilter;

	public:
		TextureParams();

		TextureParams(i32, i32, i32, i32);
		TextureParams(i32, i32);

		auto setDefaultHorzWrap(i32) -> TextureParams&;
		auto setDefaultVertWrap(i32) -> TextureParams&;
		auto setDefaultMinFilter(i32) -> TextureParams&;
		auto setDefaultMagFilter(i32) -> TextureParams&;

		auto setHorzWrap(i32) -> TextureParams&;
		auto setVertWrap(i32) -> TextureParams&;
		auto setMinFilter(i32) -> TextureParams&;
		auto setMagFilter(i32) -> TextureParams&;
		auto setFilter(i32) -> TextureParams&;
		auto setWrap(i32) -> TextureParams&;

		friend class Texture;
	};
}

#endif
