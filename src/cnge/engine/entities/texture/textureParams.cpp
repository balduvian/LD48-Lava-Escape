
#include <GL/glew.h>

#include "textureParams.h"

namespace CNGE {
	i32 TextureParams::defaultHorzWrap = GL_CLAMP_TO_EDGE;
	i32 TextureParams::defaultVertWrap = GL_CLAMP_TO_EDGE;
	i32 TextureParams::defaultMinFilter = GL_LINEAR;
	i32 TextureParams::defaultMagFilter = GL_NEAREST;

	TextureParams::TextureParams() {
		TextureParams::horzWrap = defaultHorzWrap;
		TextureParams::vertWrap = defaultVertWrap;
		TextureParams::minFilter = defaultMinFilter;
		TextureParams::magFilter = defaultMagFilter;
	}

	TextureParams::TextureParams(i32 horzWrap, i32 vertWrap, i32 minFilter, i32 magFilter)
		: horzWrap(horzWrap), vertWrap(vertWrap), minFilter(minFilter), magFilter(magFilter) {}

	TextureParams::TextureParams(i32 wrap, i32 filter)
		: horzWrap(wrap), vertWrap(wrap), minFilter(filter), magFilter(filter) {}

	auto TextureParams::setDefaultHorzWrap(i32 horzWrap) -> TextureParams& {
		TextureParams::defaultHorzWrap = horzWrap;
		return *this;
	}

	auto TextureParams::setDefaultVertWrap(i32 vertWrap) -> TextureParams& {
		TextureParams::defaultVertWrap = vertWrap;
		return *this;
	}

	auto TextureParams::setDefaultMinFilter(i32 minFilter) -> TextureParams& {
		TextureParams::defaultMinFilter = minFilter;
		return *this;
	}

	auto TextureParams::setDefaultMagFilter(i32 magFilter) -> TextureParams& {
		TextureParams::defaultMagFilter = magFilter;
		return *this;
	}

	auto TextureParams::setHorzWrap(i32 horzWrap) -> TextureParams& {
		this->horzWrap = horzWrap;
		return *this;
	}

	auto TextureParams::setVertWrap(i32 vertWrap) -> TextureParams& {
		this->vertWrap = vertWrap;
		return *this;
	}

	auto TextureParams::setMinFilter(i32 minFilter) -> TextureParams& {
		this->minFilter = minFilter;
		return *this;
	}

	auto TextureParams::setMagFilter(i32 magFilter) -> TextureParams& {
		this->magFilter = magFilter;
		return *this;
	}

	auto TextureParams::setFilter(i32 filter) -> TextureParams& {
		this->magFilter = filter;
		this->minFilter = filter;
		return *this;
	}

	auto TextureParams::setWrap(i32 wrap) -> TextureParams& {
		this->horzWrap = wrap;
		this->vertWrap = wrap;
		return *this;
	}
}
