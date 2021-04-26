#include <iostream>
#include <fstream>

#include "al/al.h"
#include "wav.h"

#pragma pack(2)

namespace CNGE {
	struct WavFileHeader {
		u32 signature;
		u32 size;
		u32 format;
	};

	struct WavFormatChunk {
		u32 id;
		u32 size;
		u16 format;
		u16 numChannels;
		u32 sampleRate;
		u32 byteRate;
		u16 blockAlign;
		u16 bitsPerSample;
	};

	struct WavSubChunk {
		u32 id;
		u32 size;
	};

	Wav::Wav(const char* path) {
		/*
		 * whatever goes on in here might as well be magic and i'm not gonna try and understand it
		 */
		std::ifstream file(path, std::ios::binary);
		if (file) {
			WavFileHeader fileHeader;
			file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

			if (fileHeader.signature == 0x46464952 && !file.eof()) {
				WavFormatChunk formatChunk;
				file.read(reinterpret_cast<char*>(&formatChunk), sizeof(formatChunk));

				if (formatChunk.id == 0x20746d66 && !file.eof()) {
					WavSubChunk subChunk = WavSubChunk();
					subChunk.size = 0;
					do {
						file.ignore(subChunk.size);
						file.read(reinterpret_cast<char*>(&subChunk), sizeof(subChunk));
					} while (subChunk.id != 0x61746164 && !file.eof());
					if (!file.eof()) {
						data = new u8[subChunk.size];
						file.read(reinterpret_cast<char*>(data), subChunk.size);
						if (!file.eof()) {
							size = subChunk.size;
							if (formatChunk.numChannels == 1)
								format = formatChunk.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
							else
								format = formatChunk.bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
							sampleRate = formatChunk.sampleRate;
						}
						else
							delete[] data;
					}
				}
			}
		}
		file.close();
	}

	Wav::~Wav() {
		//delete[] data;
	}

	auto Wav::getFormat() -> u32 {
		return format;
	}

	auto Wav::getSampleRate() -> u32 {
		return sampleRate;
	}

	auto Wav::getSize() -> u32 {
		return size;
	}

	auto Wav::getData() -> u8* {
		return data;
	}

	auto Wav::getErr() -> bool {
		return data == nullptr;
	}
}
