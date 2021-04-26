
#include "al/al.h"
#include "al/alc.h"

#include "listener.h"

namespace CNGE {
    Listener::Listener() :
        position(Vector3f()),
        velocity(Vector3f()),
        at(0, 0, -1),
        up(0, 1, 0),
        gain(1) {}

    Listener::Listener(Vector3f position, Vector3f velocity, Vector3f at, Vector3f up, float gain) :
        position(position),
        velocity(velocity),
        at(at),
        up(up),
        gain(gain) {}

    void Listener::setActive() {
        alListener3f(AL_POSITION, position.x(), position.y(), position.z());

        alListener3f(AL_VELOCITY, velocity.x(), velocity.y(), velocity.z());

        float orientation[] = { at.x(), at.y(), at.z(), up.x(), up.y(), up.z() };
        alListenerfv(AL_ORIENTATION, orientation);

        alListenerf(AL_GAIN, gain);
    }

    void Listener::setPosition(Vector3f position) {
        this->position = position;
    }

    void Listener::setVelocity(Vector3f velocity) {
        this->velocity = velocity;
    }

    void Listener::setOrientation(Vector3f at, Vector3f up) {
        this->at = at;
        this->up = up;
    }

    void Listener::setGain(float gain) {
        this->gain = gain;
    }

    Listener::~Listener() {}
}
