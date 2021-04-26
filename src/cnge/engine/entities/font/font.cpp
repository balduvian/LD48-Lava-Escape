//
// Created by Emmet on 1/10/2021.
//

#include "font.h"
#include <iostream>

namespace CNGE {
	Font::Font(const char * dataFilepath, TextureParams textureParams):
		CNLL::Resource(true),
		dataFilepath(dataFilepath),
		textureParams(textureParams),
		image(),
		texture(),
		fontData() {}

	Font::~Font() {
		onDestroy();
	}

	auto Font::customGather() -> bool {
		fontData = FontData::fromFile(dataFilepath.c_str());
		if (fontData == nullptr) return false;

		/* the font image path is relative to the res path */
		auto resPath = std::filesystem::path(dataFilepath).parent_path().string();

		image = Image::fromPNG((resPath + '/' + fontData->fontImagePath).c_str());
		return image != nullptr;
	}

	auto Font::customDiscard() -> void {
		image = nullptr;
	}

	auto Font::customLoad() -> void {
		texture = std::make_unique<Texture>(image->getWidth(), image->getHeight(), image->getPixels(), textureParams);
	}

	auto Font::customUnload() -> void {
		texture = nullptr;
	}

	auto Font::getTexture() -> Texture * {
		return texture.get();
	}

	auto Font::render(f32 x, f32 y, f32 height, const char * text, std::function<void(f32, f32, f32, f32, f32*)>&& onChar) -> void {
		auto scale = height / (f32) fontData->lineHeight;
		auto startX = x;

		for (auto i = 0, character = (i32) (u8) *text; character != 0; ++i, character = (u8) text[i]) {
			if (character == '\n') {
				x = startX;
				y -= height;

			} else {
				auto &charData = fontData->charDataList[character];

				onChar(
					x + (f32) charData.xOffset * scale,
					y - ((f32) charData.yOffset + (f32) charData.height) * scale,
					(f32) charData.width * scale,
					(f32) charData.height * scale,
					Texture::getTileValues(
						(f32) charData.width / (f32) fontData->scaleW,
						(f32) charData.height / (f32) fontData->scaleH,
						(f32) charData.x / (f32) fontData->scaleW,
						(f32) charData.y / (f32) fontData->scaleH
					)
				);

				x += (f32)charData.advance * scale;
			}
		}
	}
}
