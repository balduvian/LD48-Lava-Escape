
#ifndef CNGE_WAV_FILE
#define CNGE_WAV_FILE

#include "../types.h"

namespace CNGE {
	class Wav {
	private:
		u32 format, sampleRate, size;
		u8* data;

	public:
		Wav(const char* path);
		~Wav();

		auto getFormat() -> u32;
		auto getSampleRate() -> u32;
		auto getSize() -> u32;
		auto getData() -> u8*;

		auto getErr() -> bool;
	};
}

#endif
