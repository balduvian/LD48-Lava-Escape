//
// Created by Emmet on 1/10/2021.
//

#ifndef CNGE_8_2_FONTDATA_H
#define CNGE_8_2_FONTDATA_H

#include <vector>
#include <memory>
#include <variant>

#include "cnge/types.h"

namespace CNGE {
	struct CharData {
		u16 x;
		u16 y;
		u16 width;
		u16 height;
		i16 xOffset;
		i16 yOffset;
		i16 advance;

		CharData();
	};

	class FontData {
	public:
		FontData();

		u16 pages;

		u16 scaleW;
		u16 scaleH;

		u16 base;
		u16 lineHeight;

		std::string fontImagePath;

		CharData charDataList[256];

		static auto fromFile(const char *) -> std::unique_ptr<FontData>;
	};
}

#endif //CNGE_8_2_FONTDATA_H
