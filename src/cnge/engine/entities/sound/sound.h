
#ifndef CNGE_AUDIO_SOURCE
#define CNGE_AUDIO_SOURCE

#include "cnge/load/resource.h"

#include "cnge/types.h"
#include "../../audio/audioBuffer.h"

namespace CNGE {
	class Sound : public CNLL::Resource {
	private:
		u32 source;

		const char* wavFilePath;
		std::unique_ptr<Wav> wav;

	public:
		Sound(const char*);
		
		auto play() -> void;
		auto pause() -> void;
		auto stop() -> void;
		
		auto loop(bool = true) -> void;
		
		auto customGather() -> bool override;
		auto customDiscard() -> void override;
		auto customLoad() -> void override;
		auto customUnload() -> void override;

		auto setVolume(f32) -> void;

		~Sound(); 
	};
}

#endif
