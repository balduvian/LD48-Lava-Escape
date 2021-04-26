
#include <iostream>

#include <al\al.h>

#include "sound.h"

namespace CNGE {
	Sound::Sound(const char* wavFilePath)
		: Resource(true), wavFilePath(wavFilePath), wav(), source() {}

	auto Sound::customGather() -> bool {
		wav = std::make_unique<Wav>(wavFilePath);

		return !wav->getErr();
	}

	auto Sound::customDiscard() -> void {
		wav = nullptr;
	}

	auto Sound::customLoad() -> void {
		auto buffer = AudioBuffer(wav.get());

		alGenSources(1, &source);
		alSourcef(source, AL_GAIN, 1);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_DIRECTION, 0, 0, 0);
		alSourcei(source, AL_BUFFER, buffer.getBuffer());
	}

	auto Sound::customUnload() -> void {
		alDeleteSources(1, &source);
	}

	auto Sound::play() -> void {
		alSourcePlay(source);
	}

	auto Sound::stop() -> void {
		alSourceStop(source);
	}

	auto Sound::pause() -> void {
		alSourcePause(source);
	}

	auto Sound::loop(bool loop) -> void {
		alSourcei(source, AL_LOOPING, loop);
	}

	auto Sound::setVolume(f32 volume) -> void {
		alSourcef(source, AL_GAIN, volume);
	}

	Sound::~Sound() {
		onDestroy();
	}
}
