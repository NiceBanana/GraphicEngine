//
// Created by student on 04.03.22.
//

#ifndef ENGINE_LINE2D_H
#define ENGINE_LINE2D_H


#include "Point2D.h"
#include "Color.h"

class Line2D {
public:
    Point2D p1;
    Point2D p2;
    Color color;
    double z0;
    double z1;

    Line2D(Point2D p1, Point2D p2, const Color &color);

    Line2D();

    virtual ~Line2D();
};


#endif //ENGINE_LINE2D_H
