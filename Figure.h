//
// Created by student on 18.03.22.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H


#include <vector>

#include "vector3d.h"
#include "Face.h"
#include "Color.h"

class Figure {
public:
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color color;
    double reflectionCoefficient;
    Color ambientReflection;
    Color diffuseReflection;
    Color specularReflection;

    void triangulate();
};


#endif //ENGINE_FIGURE_H
