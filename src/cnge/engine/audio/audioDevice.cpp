
#include <iostream>
#include <cstring>

#include "audioDevice.h"

namespace CNGE {
	AudioOutputDevice::AudioOutputDevice(ALCdevice* device, const char* name) {
		this->device = device;
		context = alcCreateContext(device, 0);
		auto nameLength = strlen(name);
		this->name = new char[nameLength + 1];
		strcpy_s(this->name, nameLength + 1, name);
	}

	void AudioOutputDevice::setActive() {
		alcMakeContextCurrent(context);
	}

	const char* AudioOutputDevice::getName() {
		return name;
	}

	AudioOutputDevice::~AudioOutputDevice() {
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	i32 AudioDevices::numOutputDevices = 0;
	AudioOutputDevice** AudioDevices::outputDevices = 0;

	void AudioDevices::init() {
		bool enumOk = alcIsExtensionPresent(0, "ALC_ENUMERATION_EXT");
		if (enumOk) {
			numOutputDevices = 0;
			const char* devices = alcGetString(0, ALC_DEVICE_SPECIFIER);
			const char* loop = devices;
			while (*loop) {
				++numOutputDevices;
				loop += strlen(loop) + 1;
			}
			outputDevices = new AudioOutputDevice * [numOutputDevices];
			loop = devices;
			for (auto i = 0; i < numOutputDevices; ++i) {
				outputDevices[i] = new AudioOutputDevice(alcOpenDevice(loop), loop);
				loop += strlen(loop) + 1;
			}
		}
		else {
			ALCdevice* device = alcOpenDevice(0);
			if (device) {
				numOutputDevices = 1;
				outputDevices = new AudioOutputDevice * [1]{ new AudioOutputDevice(device, alcGetString(device, ALC_DEVICE_SPECIFIER)) };
			}
			else {
				numOutputDevices = 0;
				outputDevices = 0;
			}
		}
	}

	u32 AudioDevices::getNumOutputDevices() {
		return numOutputDevices;
	}

	AudioOutputDevice* AudioDevices::getDefaultOutputDevice() {
		return outputDevices[0];
	}

	AudioOutputDevice* AudioDevices::getOutputDevice(u32 outputDevice) {
		if (outputDevice >= 0 && outputDevice < numOutputDevices)
			return outputDevices[outputDevice];
		else
			return 0;
	}

	void AudioDevices::cleanup() {
		for (i32 i = 0; i < numOutputDevices; ++i)
			delete outputDevices[i];
		delete[] outputDevices;
	}

	AudioDevices::AudioDevices() {}
	AudioDevices::~AudioDevices() {}
}
