//
// Created by student on 04.03.22.
//

#include "Color.h"

Color::Color(double red, double green, double blue) : red(red),green(green),blue(blue)  {}

Color::Color() {}

Color::~Color() {}

Color Color::operator+(const Color& kleur){
    red+=kleur.red;
    green+=kleur.green;
    blue+=kleur.blue;
    return *this;
}
