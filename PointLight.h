//
// Created by dante on 20/05/22.
//

#ifndef ENGINE_POINTLIGHT_H
#define ENGINE_POINTLIGHT_H

#include "vector3d.h"
#include "Light.h"

class PointLight: public Light{
public:
    Vector3D location;
    double spotAngle;
};


#endif //ENGINE_POINTLIGHT_H
