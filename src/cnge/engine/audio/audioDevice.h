
#ifndef CNGE_AUDIO_DEVICE
#define CNGE_AUDIO_DEVICE

#include <al/al.h>
#include <al/alc.h>

#include "cnge/types.h"

namespace CNGE {
	class AudioOutputDevice {
	private:
		AudioOutputDevice(ALCdevice* device, const char* name);
		ALCdevice* device;
		ALCcontext* context;
		char* name;

	public:
		void setActive();
		const char* getName();
		~AudioOutputDevice();

		friend class AudioDevices;
	};

	class AudioDevices {
	public:
		static i32 numOutputDevices;
		static AudioOutputDevice** outputDevices;

		AudioDevices();
		~AudioDevices();

		static void init();
		static void cleanup();
	
		static u32 getNumOutputDevices();
		static AudioOutputDevice* getDefaultOutputDevice();
		static AudioOutputDevice* getOutputDevice(u32 outputDevice);
	};
}

#endif
