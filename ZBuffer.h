//
// Created by student on 22.04.22.
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H

#include <vector>
#include "Color.h"
#include "easy_image.h"
#include "vector3d.h"
#include "Light.h"
#include "InfLight.h"

typedef std::list<Light> Lights3D;
typedef std::list<InfLight> InfLights3D;

class Point2D;
class ZBuffer: public std::vector<std::vector<double>>{
public:
    ZBuffer(const int breedte,const int hoogte);
    static void draw_zbuf_line(ZBuffer &, img::EasyImage &, const unsigned int x0, const unsigned int y0, const double z0, const unsigned int x1, const unsigned int y1, const double z1, img::Color color);
    static void draw_zbuf_triangle(ZBuffer &buffer, img::EasyImage &image,Vector3D &A,Vector3D &B, Vector3D &C,double d,double dx,double dy,Color ambientReflection, Color diffuseReflection, Color specularReflection,double reflectionCoeff, Lights3D &lights, InfLights3D &infLichten);
    static Point2D projection4Triangle(Vector3D &punt,const double &d,const double &dx, const double &dy);
    static Color calculateAmbient(const Color& ambientReflection,const Lights3D& lichten);
    static Color calculateDiffuse(const Color& diffuseReflection, const InfLights3D& infLichten,Vector3D W);
    static Color limitKleur(Color kleur);
};


#endif //ENGINE_ZBUFFER_H
