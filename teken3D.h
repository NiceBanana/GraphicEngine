//
// Created by student on 18.03.22.
//

#ifndef ENGINE_TEKEN3D_H
#define ENGINE_TEKEN3D_H

#include <cmath>

#include "teken2D.h"
#include "vector3d.h"
#include "Face.h"
#include "Figure.h"

using Figures3D=std::list<Figure>;

class teken3D {
public:
    static Matrix matrixScale(const double scale);

    static Matrix matrixRotateX(double angle);

    static Matrix matrixRotateY(double angle);

    static Matrix matrixRotateZ(double angle);

    static Matrix matrixTranslate(Vector3D &vector);

    static void matrixTransform(Figure &fig, Matrix &m);

    static Matrix matrixEye(Vector3D &eyePoint);

    static void matrixTransformAll(Figures3D &figs, Matrix &m);

    static Lines2D projectionAll(Figures3D &figs);

    static Point2D projection(Vector3D &point, double d=1);

    static std::string transformString(const LParser::LSystem3D &lsysteem);

    static Figure stringToFigure(const std::string& stringToDraw, const LParser::LSystem3D &lsysteem);
};


#endif //ENGINE_TEKEN3D_H
