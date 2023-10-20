//
// Created by student on 11.03.22.
//

#include "teken3D.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <list>

using Lines2D=std::list<Line2D>;
using Figures3D=std::list<Figure>;

int main(){
    teken3D::matrixScale(3);
    teken3D::matrixRotateX(90);
    teken3D::matrixRotateX(0);
    teken3D::matrixRotateY(90);
    teken3D::matrixRotateY(0);
    teken3D::matrixRotateZ(90);
    teken3D::matrixRotateZ(0);
    Vector3D vector=Vector3D::vector(4,2,0);
    teken3D::matrixTranslate(vector);

}

