//
// Created by dante on 20/05/22.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include <list>
#include "Color.h"

class Light {
public:
    Color ambientLight;
    Color diffuseLight;
    Color specularLight;
};


#endif //ENGINE_LIGHT_H
