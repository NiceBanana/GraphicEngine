#include "easy_image.h"
#include "ini_configuration.h"
#include "Inleiding.cpp"
#include "Color.h"
#include "Point2D.h"
#include "Line2D.h"
#include "l_parser.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <list>

using Lines2D=std::list<Line2D>;

//vergelijkt de coordinaten van de punten in MinMax
void Compare(Point2D punt,double &xmax, double &xmin, double &ymax, double &ymin){
    if(punt.x>xmax){
        xmax=punt.x;
    }
    else if(punt.x<xmin){
        xmin=punt.x;
    }
    if(punt.y>ymax){
        ymax=punt.y;
    }
    else if(punt.y<ymin){
        ymin=punt.y;
    }
}

//berekent de uiterste punten van de tekening om zo afmetingen van image te verkrijgen
std::vector<double> MinMax(Lines2D &lines){
    double xmax=lines.front().p1.x;
    double xmin=xmax;
    double ymax=lines.front().p1.y;
    double ymin=ymax;
    for(auto &lijn:lines) {
        Compare(lijn.p1,xmax,xmin,ymax,ymin);
        Compare(lijn.p2,xmax,xmin,ymax,ymin);
    }
    std::vector<double> bounds={xmax,xmin,ymax,ymin};
    return bounds;
}

//berkent het midden van de virtuele tekening
Point2D berekenMidden(double d, std::vector<double> bounds){
    double DCx=d*(bounds[0]+bounds[1])/2;
    double DCy=d*(bounds[2]+bounds[3])/2;
    Point2D middle=Point2D(DCx,DCy);
    return middle;
}

//berekent de belangrijke afmetingen
std::vector<double> BerekenAfmetingen(Lines2D &lines,const int size,std::vector<double> bounds){
    double xRange=bounds[0]-bounds[1];
    double yRange=bounds[2]-bounds[3];
    double imageX=size*xRange/std::max(xRange,yRange);
    double imageY=size*yRange/std::max(xRange,yRange);
    std::vector<double> afmetingen={xRange, yRange, imageX, imageY};
    return afmetingen;
}

//schalen van coordinaten van een lijn
double scale(Lines2D &lines,std::vector<double> afmetingen){
    double d=0.95*(afmetingen[2]/afmetingen[0]);
    for(auto &lijn:lines){
        lijn.p1.x*=d;
        lijn.p1.y*=d;
        lijn.p2.x*=d;
        lijn.p2.y*=d;
    }
    return d;
}

//afronden coordinaten van een lijn
void rondAf(Lines2D &lines){
    for(auto &lijn:lines){
        lijn.p1.x=std::lround(lijn.p1.x);
        lijn.p1.y=std::lround(lijn.p1.y);
        lijn.p2.x=std::lround(lijn.p2.x);
        lijn.p2.y=std::lround(lijn.p2.y);
    }
}

//verschuift de virtuele afbeelding naar juiste positie in image
void shiftImage(Lines2D &lines, std::vector<double> afmetingen, Point2D midden){
    double dx=(afmetingen[2]/2)-midden.x;
    double dy=(afmetingen[3]/2)-midden.y;
    for(auto &lijn:lines){
        lijn.p1.x+=dx;
        lijn.p1.y+=dy;
        lijn.p2.x+=dx;
        lijn.p2.y+=dy;
    }
}

//kalibreert de kleuren op de juiste waarden
img::Color fixColor(double rood, double groen, double blauw){
    int red=std::lround(rood*255);
    int green=std::lround(groen*255);
    int blue=std::lround(blauw*255);
    img::Color kleur=img::Color(red,green,blue);
    return kleur;
}

//hoofddeel waar alle hulpfuncties samenkomen om tekening te maken
img::EasyImage generateLines(Lines2D &lines, const int size,std::vector<double> bgkleur){
    std::vector<double> bounds=MinMax(lines);
    std::vector<double> afmetingen=BerekenAfmetingen(lines,size,bounds);
    double schaalfactor=scale(lines,afmetingen);
    shiftImage(lines,afmetingen,berekenMidden(schaalfactor,bounds));
    rondAf(lines);
    unsigned int breedte=abs(std::lround(afmetingen[2]));
    unsigned int hoogte=abs(std::lround(afmetingen[3]));
    img::Color color=fixColor(bgkleur[0],bgkleur[1],bgkleur[2]);
    img::EasyImage image(breedte,hoogte,color);
    for(auto &lijn:lines){
        unsigned int x0=std::lround(lijn.p1.x);
        unsigned int y0=std::lround(lijn.p1.y);
        unsigned int x1=std::lround(lijn.p2.x);
        unsigned int y1=std::lround(lijn.p2.y);
        img::Color kleur=fixColor(lijn.color.red,lijn.color.green,lijn.color.blue);
        image.draw_line(x0,y0,x1,y1,kleur);
    }
    return image;
}

//vormt de initiele string om en herhaalt dat zoveel keer als er iterations zijn
std::string transformString(const LParser::LSystem2D &lsysteem){
    std::string current=lsysteem.get_initiator();
    std::string next;
    std::set<char> alfabet=lsysteem.get_alphabet();
    for(int i=0;i<lsysteem.get_nr_iterations();i++){
        for(auto symbool:current){
            if(alfabet.find(symbool)!=alfabet.end()){
                next+=lsysteem.get_replacement(symbool);
            }
            else{
                next+=symbool;
            }
        }
        current=next;
        next="";
    }
    return current;
}

//zet de string om te tekenen om in de verzameling lijnen om te tekenen
Lines2D stringToLines(std::string stringToDraw, const LParser::LSystem2D &lsysteem, const Color &lijnkleur){
    Lines2D linesToDraw;
    double currentAngle=(lsysteem.get_starting_angle()*M_PI)/180;
    double changeAngle=(lsysteem.get_angle()*M_PI)/180;
    std::set<char> alfabet=lsysteem.get_alphabet();
    Point2D position_current=Point2D(0,0);
    Point2D position_last=Point2D(0,0);
    for(auto symbool:stringToDraw){
        if(symbool=='+'){
            currentAngle+=changeAngle;
        }
        else if(symbool=='-'){
            currentAngle-=changeAngle;
        }
        else if(alfabet.find(symbool)!=alfabet.end()){
            if(lsysteem.draw(symbool)){
                position_current.x+=std::cos(currentAngle);
                position_current.y+=std::sin(currentAngle);
                Line2D lijn= Line2D(position_last,position_current,lijnkleur);
                linesToDraw.push_back(lijn);
            }
            else{
                position_current.x+=std::cos(currentAngle);
                position_current.y+=std::sin(currentAngle);
            }
            position_last=position_current;
        }
    }
    return linesToDraw;
}

img::EasyImage generate_image(const ini::Configuration &configuration)
{
    std::string name= configuration["General"]["type"].as_string_or_die();
    int size= configuration["General"]["size"].as_int_or_die();
    std::vector<double> kleur_achtergrond= configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
    std::string L2Dfile= configuration["2DLSystem"]["inputfile"].as_string_or_die();
    std::vector<double> kleur_figuur= configuration["2DLSystem"]["color"].as_double_tuple_or_die();
    LParser::LSystem2D l_system;
    std::ifstream input_stream(L2Dfile);
    input_stream >> l_system;
    input_stream.close();
    Color lijnkleur= Color(kleur_figuur[0],kleur_figuur[1],kleur_figuur[2]);
    Lines2D linesToDraw=stringToLines(transformString(l_system),l_system,lijnkleur); //
    img::EasyImage image=generateLines(linesToDraw,size,kleur_achtergrond);
    return image;
}

int main(int argc, char const* argv[])
{
        int retVal = 0;
        try
        {
                std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
                if (args.empty()) {
                        std::ifstream fileIn("filelist");
                        std::string filelistName;
                        while (std::getline(fileIn, filelistName)) {
                                args.push_back(filelistName);
                        }
                }
                for(std::string fileName : args)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(fileName);
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cout << "Could not generate image for " << fileName << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }
        return retVal;
}
