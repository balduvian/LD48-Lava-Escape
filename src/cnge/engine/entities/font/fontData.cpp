//
// Created by Emmet on 1/10/2021.
//

#include <fstream>
#include <cstring>
#include <iostream>

#include "fontData.h"

namespace CNGE {
	CharData::CharData() : x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), advance(0) {}
	FontData::FontData() : charDataList() {}

	constexpr auto BLOCK_HEADER_SIZE = 5;

	template<typename T>
	auto readInt(u8 * filePtr) {
		return *reinterpret_cast<T*>(filePtr);
	}

	auto endOfFile(u8 * filePtr, std::vector<u8> & fileBuffer) {
		return filePtr - fileBuffer.data() > fileBuffer.size();
	}

	/**
	 * @return a unique pointer to the fontdata,
	 *         or a c string with the error message
	 */
	auto FontData::fromFile(const char *filepath) -> std::unique_ptr<FontData> {
		auto fileStream = std::fstream(filepath, std::fstream::in | std::fstream::binary);
		if (!fileStream.good()) return nullptr;

		auto fontData = std::make_unique<FontData>();

		/* read the entire file into a buffer */
		fileStream.seekg(0, std::ios::end);
		auto fileBuffer = std::vector<u8>(fileStream.tellg());
		fileStream.seekg(0, std::ios::beg);
		fileStream.read(reinterpret_cast<char*>(fileBuffer.data()), fileBuffer.size());

		auto filePtr = fileBuffer.data();

		/* check if this is a valid BMF binary file in version 3 */
		if (fileBuffer.size() < 4) return nullptr;
		if (std::memcmp(filePtr, "BMF\3", 4) != 0) return nullptr;
		filePtr += 4;

		/* read each block in the binary file */
		while (!endOfFile(filePtr + BLOCK_HEADER_SIZE, fileBuffer)) {
			/* byte 0: block id */
			/* byte 1-4: block size in bytes */
			auto blockId = *filePtr;
			auto blockSize = readInt<u32>(filePtr + 1);
			filePtr += BLOCK_HEADER_SIZE;

			if (endOfFile(filePtr + blockSize, fileBuffer))
				return nullptr;

			switch (blockId) {
				/* no info from info block */
				case 1: break;
				/* common block */
				case 2: {
					fontData->lineHeight = readInt<u16>(filePtr);
					fontData->base = readInt<u16>(filePtr + 2);
					fontData->scaleW = readInt<u16>(filePtr + 4);
					fontData->scaleH = readInt<u16>(filePtr + 6);
					fontData->pages = readInt<u16>(filePtr + 8);

					if (fontData->pages > 1) return nullptr;

					break;
				}
				/* pages */
				case 3: {
					/* safety check if there is a null terminated string */
					auto stringSentry = filePtr;
					while (*(stringSentry++) != '\0')
						if (endOfFile(stringSentry, fileBuffer))
							return nullptr;

					fontData->fontImagePath = std::string(reinterpret_cast<char*>(filePtr));

					break;
				}
				/* char */
				case 4: {
					auto numChars = blockSize / 20;

					for (auto i = 0; i < numChars; ++i) {
						auto charID = readInt<u32>(filePtr + 20 * i);

						/* only record ascii values */
						if (charID < 256) {
							auto charData = fontData->charDataList + charID;

							charData->x = readInt<u16>(filePtr + (20 * i) + 4);
							charData->y = readInt<u16>(filePtr + (20 * i) + 6);
							charData->width = readInt<u16>(filePtr + (20 * i) + 8);
							charData->height = readInt<u16>(filePtr + (20 * i) + 10);
							charData->xOffset = readInt<i16>(filePtr + (20 * i) + 12);
							charData->yOffset = readInt<i16>(filePtr + (20 * i) + 14);
							charData->advance = readInt<i16>(filePtr + (20 * i) + 16);
						}
					}

					break;
				}
				case 5: break;
				default: return nullptr;
			}

			filePtr += blockSize;
		}

		return fontData;
	}
}
