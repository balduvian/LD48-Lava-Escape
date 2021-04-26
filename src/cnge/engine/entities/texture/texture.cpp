
#include <iostream>
#include "GL/glew.h"
#include "texture.h"

namespace CNGE {
	float Texture::tileValues[4]{ 1, 1, 0, 0 };
	const float Texture::DEFAULT_TILE_VALUES[4]{ 1, 1, 0, 0 };

	/* loading */

	/**
	 * important! texture does not manage the pixel data you pass into it
	 *
	 * @param width
	 * @param height
	 * @param pixels
	 * @param params
	 */
	Texture::Texture(u32 width, u32 height, u8* pixels, TextureParams params)
		: width(width), height(height), texture(),
		horzWrap(params.horzWrap), vertWrap(params.vertWrap),
		minFilter(params.minFilter), magFilter(params.magFilter) {

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horzWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	/* use */

	auto Texture::bind(i32 slot) -> void {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	auto Texture::get() -> u32 {
		return texture;
	}

	Texture::~Texture() {
		glDeleteTextures(1, &texture);
	}

	auto Texture::getTileValues(f32 x, f32 y, f32 w, f32 h) -> f32 * {
		tileValues[0] = x;
		tileValues[1] = y;
		tileValues[2] = w;
		tileValues[3] = h;

		return tileValues;
	}
}
