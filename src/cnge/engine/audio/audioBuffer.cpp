
#include "al/al.h"
#include "audioBuffer.h"

namespace CNGE {
    void AudioBuffer::clean() {
        if (buffer)
            alDeleteBuffers(1, &buffer);
    }

    AudioBuffer::AudioBuffer(u32 format, u32 size, u8 data[], u32 frequency) {
        clean();
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, size, frequency);
    }

    AudioBuffer::AudioBuffer(Wav* wavFile) {
        bufferData(wavFile->getFormat(), wavFile->getSize(), wavFile->getData(), wavFile->getSampleRate());
    }

    AudioBuffer::AudioBuffer(const char* path) {
        auto file = new Wav(path);
        loadWav(file);
        delete file;
    }

    void AudioBuffer::bufferData(u32 format, u32 size, u8 data[], u32 frequency) {
        clean();
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, size, frequency);
    }

    void AudioBuffer::loadWav(Wav* wavFile) {
        bufferData(wavFile->getFormat(), wavFile->getSize(), wavFile->getData(), wavFile->getSampleRate());
    }

    void AudioBuffer::loadWav(const char* path) {
        Wav* file = new Wav(path);
        loadWav(file);
        delete file;
    }

    u32 AudioBuffer::getBuffer() {
        return buffer;
    }

    AudioBuffer::~AudioBuffer() {
        clean();
    }
}