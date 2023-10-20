//
// Created by dante on 20/05/22.
//

#ifndef ENGINE_INFLIGHT_H
#define ENGINE_INFLIGHT_H

#include "vector3d.h"
#include "Light.h"

class InfLight: public Light{
public:
    Vector3D ldVector;

    InfLight(const Vector3D &Ld);
};


#endif //ENGINE_INFLIGHT_H
