//
// Created by student on 04.03.22.
//

#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H


class Color {
public:
    double red;
    double blue;
    double green;

    Color(double red, double green, double blue);

    Color();

    virtual ~Color();

    Color operator+(const Color& kleur);
};


#endif //ENGINE_COLOR_H
