//
// Created by student on 04.03.22.
//

#ifndef INLEIDING_INCLUDED
#define INLEIDING_INCLUDED

#include "easy_image.h"
#include "ini_configuration.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <utility>

img::EasyImage GenerateRectangle(unsigned int breedte, unsigned int hoogte){
    img::EasyImage image(breedte,hoogte);
    for(unsigned int i = 0; i < breedte; i++)
    {
        for(unsigned int j = 0; j < hoogte; j++)
        {
            image(i,j).red = i;
            image(i,j).green = j;
            image(i,j).blue = (i+j)%breedte;
        }
    }
    std::ofstream fout("out.bmp", std::ios::binary);
    fout << image;
    fout.close();
    return image;
}

img::EasyImage GenerateDambord(unsigned int breedte, unsigned int hoogte, unsigned int aantalx, unsigned int aantaly, std::vector<double> wit, std::vector<double> zwart){
    unsigned int W=breedte/aantalx;
    unsigned int H=hoogte/aantaly;
    img::EasyImage image(breedte,hoogte);
    for(unsigned int i = 0; i < breedte; i++)
    {
        for(unsigned int j = 0; j < hoogte; j++)
        {

            unsigned int Bx= i/W;
            unsigned int By=j/H;
            if((Bx+By)%2==0){
                image(i,j).red = std::lround(wit[0]*255);
                image(i,j).green = std::lround(wit[1]*255);
                image(i,j).blue = std::lround(wit[2]*255);
            }
            else{
                image(i,j).red = std::lround(zwart[0]*255);
                image(i,j).green = std::lround(zwart[1]*255);
                image(i,j).blue = std::lround(zwart[2]*255);
            }
        }
    }
    return image;
}

img::EasyImage GenerateQuarterCircle(unsigned int breedte, unsigned int hoogte, unsigned int aantal,std::vector<double> kleur_lijn, std::vector<double> kleur_achtergrond){
    img::EasyImage image(breedte,hoogte);
    img::Color kleur=img::Color(std::lround(kleur_lijn[0]*255),std::lround(kleur_lijn[1]*255),std::lround(kleur_lijn[2]*255));
    unsigned int Hs= hoogte/(aantal-1);
    unsigned int Ws=breedte/(aantal-1);
    for(int i=0;i<breedte;i+=Hs){
        image.draw_line(1, i, i,hoogte-1, kleur);
        image.draw_line(0,hoogte-1,breedte-1,hoogte-1,kleur);
    }
    return image;
}

#endif