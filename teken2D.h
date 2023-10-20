//
// Created by student on 18.03.22.
//

#ifndef ENGINE_TEKEN2D_H
#define ENGINE_TEKEN2D_H

#include "easy_image.h"
#include "Color.h"
#include "Point2D.h"
#include "Line2D.h"
#include "l_parser.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <list>

using Lines2D=std::list<Line2D>;

class teken2D {
public:
    static void Compare(Point2D punt,double &xmax, double &xmin, double &ymax, double &ymin);

    static std::vector<double> MinMax(Lines2D &lines);

    static Point2D berekenMidden(double d, std::vector<double> bounds);

    static std::vector<double> BerekenAfmetingen(Lines2D &lines,const int size,std::vector<double> bounds);

    static double scale(Lines2D &lines,std::vector<double> afmetingen);

    static void rondAf(Lines2D &lines);

    static void shiftImage(Lines2D &lines, std::vector<double> afmetingen, Point2D midden);

    static img::Color fixColor(double rood, double groen, double blauw);

    static img::EasyImage generateLines(Lines2D &lines, const int size,std::vector<double> bgkleur, bool Zbuff);

    static std::string transformString(const LParser::LSystem2D &lsysteem);

    static Lines2D stringToLines(std::string stringToDraw, const LParser::LSystem2D &lsysteem, const Color &lijnkleur);

    static std::pair<unsigned int,unsigned int> values4Triangle(Lines2D &lines, int &size, double &d, double &dx, double &dy);
};


#endif //ENGINE_TEKEN2D_H
