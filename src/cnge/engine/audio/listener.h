
#ifndef CNGE_LISTENER
#define CNGE_LISTENER

#include "cnge/math/vector3.h"

namespace CNGE {
    class Listener {
    private:
        Vector3f position, velocity, at, up;
        float gain;

    public:
        Listener();
        Listener(Vector3f position, Vector3f velocity, Vector3f at, Vector3f up, float gain);
        void setActive();
        void setPosition(Vector3f position);
        void setVelocity(Vector3f velocity);
        void setOrientation(Vector3f at, Vector3f up);
        void setGain(float gain);
        ~Listener();
    };
}

#endif
