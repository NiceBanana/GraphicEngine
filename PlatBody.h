//
// Created by student on 25.03.22.
//

#ifndef ENGINE_PLATBODY_H
#define ENGINE_PLATBODY_H

#include "teken3D.h"

class PlatBody {
public:
    static Figure generateCube();
    static Figure generateTetrahedron();
    static Figure generateOctahedron();
    static Figure generateIcosahedron();
    static Figure generateDodecahedron();
    static Figure generateSphere(const double radius, const int n);
    static void splitTriangle(Figure &Cirkel,std::vector<Face> &newTriangles,Face &vlak);
    static Figure generateCylinder(const int n, const double h);
    static Figure generateCone(const int n,const double h);
    static Figure generateTorus(const double R, const double r, const int n, const int m);
    static void generateFractal(Figure &fig, Figures3D &fractal, int nr_iterations, double scale);
};


#endif //ENGINE_PLATBODY_H
