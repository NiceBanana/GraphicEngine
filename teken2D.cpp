//
// Created by student on 18.03.22.
//

#include "teken2D.h"
#include "ZBuffer.h"

//vergelijkt de coordinaten van de punten in MinMax
void teken2D::Compare(Point2D punt,double &xmax, double &xmin, double &ymax, double &ymin){
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
std::vector<double> teken2D::MinMax(Lines2D &lines){
    //std::cout<<lines.size()<<std::endl;
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
Point2D teken2D::berekenMidden(double d, std::vector<double> bounds){
    double DCx=d*(bounds[0]+bounds[1])/2;
    double DCy=d*(bounds[2]+bounds[3])/2;
    Point2D middle=Point2D(DCx,DCy);
    return middle;
}

//berekent de belangrijke afmetingen
std::vector<double> teken2D::BerekenAfmetingen(Lines2D &lines,const int size,std::vector<double> bounds){
    double xRange=bounds[0]-bounds[1];
    double yRange=bounds[2]-bounds[3];
    double imageX=size*xRange/std::max(xRange,yRange);
    double imageY=size*yRange/std::max(xRange,yRange);
    std::vector<double> afmetingen={xRange, yRange, imageX, imageY};
    return afmetingen;
}

//schalen van coordinaten van een lijn
double teken2D::scale(Lines2D &lines,std::vector<double> afmetingen){
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
void teken2D::rondAf(Lines2D &lines){
    for(auto &lijn:lines){
        lijn.p1.x=std::lround(lijn.p1.x);
        lijn.p1.y=std::lround(lijn.p1.y);
        lijn.p2.x=std::lround(lijn.p2.x);
        lijn.p2.y=std::lround(lijn.p2.y);
    }
}

//verschuift de virtuele afbeelding naar juiste positie in image
void teken2D::shiftImage(Lines2D &lines, std::vector<double> afmetingen, Point2D midden){
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
img::Color teken2D::fixColor(double rood, double groen, double blauw){
    int red=std::lround(rood*255);
    int green=std::lround(groen*255);
    int blue=std::lround(blauw*255);
    img::Color kleur=img::Color(red,green,blue);
    return kleur;
}

//hoofddeel waar alle hulpfuncties samenkomen om tekening te maken
img::EasyImage teken2D::generateLines(Lines2D &lines, const int size,std::vector<double> bgkleur, bool Zbuff){
    std::vector<double> bounds=MinMax(lines);
    std::vector<double> afmetingen=BerekenAfmetingen(lines,size,bounds);
    double schaalfactor=scale(lines,afmetingen);
    shiftImage(lines,afmetingen,berekenMidden(schaalfactor,bounds));
    rondAf(lines);
    unsigned int breedte=abs(std::lround(afmetingen[2]));
    unsigned int hoogte=abs(std::lround(afmetingen[3]));
    img::Color color=fixColor(bgkleur[0],bgkleur[1],bgkleur[2]);
    img::EasyImage image(breedte,hoogte,color);
    ZBuffer buffer=ZBuffer(breedte,hoogte);
    for(auto &lijn:lines){
        unsigned int x0=std::lround(lijn.p1.x);
        unsigned int y0=std::lround(lijn.p1.y);
        unsigned int x1=std::lround(lijn.p2.x);
        unsigned int y1=std::lround(lijn.p2.y);
        img::Color kleur=fixColor(lijn.color.red,lijn.color.green,lijn.color.blue);
        if(Zbuff){
            double z0=lijn.z0;
            double z1=lijn.z1;
            ZBuffer::draw_zbuf_line(buffer,image,x0,y0,z0,x1,y1,z1,kleur);
        }
        else {
            image.draw_line(x0, y0, x1, y1, kleur);
        }
    }
    return image;
}

//vormt de initiele string om en herhaalt dat zoveel keer als er iterations zijn
std::string teken2D::transformString(const LParser::LSystem2D &lsysteem){
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
Lines2D teken2D::stringToLines(std::string stringToDraw, const LParser::LSystem2D &lsysteem, const Color &lijnkleur){
    Lines2D linesToDraw;
    double currentAngle=(lsysteem.get_starting_angle()*M_PI)/180;
    double changeAngle=(lsysteem.get_angle()*M_PI)/180;
    std::set<char> alfabet=lsysteem.get_alphabet();
    Point2D position_current=Point2D(0,0);
    Point2D position_last=Point2D(0,0);
    std::vector<Point2D> savedPos;
    std::vector<double> savedAngle;
    for(auto symbool:stringToDraw){
        if(symbool=='+'){
            currentAngle+=changeAngle;
        }
        else if(symbool=='-'){
            currentAngle-=changeAngle;
        }
        else if(symbool=='('){
            savedPos.push_back(position_current);
            savedAngle.push_back(currentAngle);
        }
        else if(symbool==')'){
            int size=savedPos.size();
            position_current=savedPos[size-1];
            int size2=savedAngle.size();
            currentAngle=savedAngle[size2-1];
            savedPos.pop_back();
            savedAngle.pop_back();
        }
        else if(alfabet.find(symbool)!=alfabet.end()){
            position_last=position_current;
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
        }
    }
    return linesToDraw;
}

std::pair<unsigned int,unsigned int> teken2D::values4Triangle(Lines2D &lines, int &size, double &d, double &dx, double &dy) {
    std::vector<double> bounds=MinMax(lines);
    std::vector<double> afmetingen=BerekenAfmetingen(lines,size,bounds);
    d=scale(lines,afmetingen);
    Point2D midden=berekenMidden(d,bounds);
    dx=(afmetingen[2]/2)-midden.x;
    dy=(afmetingen[3]/2)-midden.y;
    unsigned int breedte=abs(std::lround(afmetingen[2]));
    unsigned int hoogte=abs(std::lround(afmetingen[3]));
    return {breedte,hoogte};
}

