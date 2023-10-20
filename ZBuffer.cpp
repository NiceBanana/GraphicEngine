//
// Created by student on 22.04.22.
//

#include "ZBuffer.h"
#include "easy_image.h"
#include "teken2D.h"
#include "InfLight.h"

#include <limits>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <utility>
#include <map>


ZBuffer::ZBuffer(const int breedte, const int hoogte) {
    std::vector<double> column;
    column.resize(hoogte,std::numeric_limits<double>::infinity());
    this->resize(breedte,column);
}

void ZBuffer::draw_zbuf_line(ZBuffer &buffer, img::EasyImage &image, unsigned int x0, unsigned int y0, double z0,
                              unsigned int x1, unsigned int y1, double z1, img::Color color) {
    double zValue;
    if (x0 == x1)
    {
        //special case for x0 == x1
        for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            double a=std::max(y0, y1);
            zValue=(i/a)/z0+(1-i/a)/z1;
            if(zValue<buffer[x0][i] or buffer[x0][i]==std::numeric_limits<double>::infinity()){
                buffer[x0][i]=zValue;
                image(x0, i) = color;
            }
        }
    }
    else if (y0 == y1)
    {
        //special case for y0 == y1
        for (unsigned int i = std::min(x0, x1); i <= std::max(x0, x1); i++)
        {
            double a=std::max(x0, x1);
            zValue=(i/a)/z0+(1-i/a)/z1;
            if(zValue<buffer[i][y0] or buffer[i][y0]==std::numeric_limits<double>::infinity()) {
                buffer[i][y0] = zValue;
                image(i, y0) = color;
            }
        }
    }
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0,z1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            for (unsigned int i = 0; i <= (x1 - x0); i++)
            {
                double a=(x1 - x0);
                zValue=(i/a)/z0+(1-i/a)/z1;
                if(zValue<buffer[x0 + i][(unsigned int) round(y0 + m * i)] or buffer[x0 + i][(unsigned int) round(y0 + m * i)]==std::numeric_limits<double>::infinity()) {
                    buffer[x0 + i][(unsigned int) round(y0 + m * i)] = zValue;
                    image(x0 + i, (unsigned int) round(y0 + m * i)) = color;
                }
            }
        }
        else if (m > 1.0)
        {
            for (unsigned int i = 0; i <= (y1 - y0); i++)
            {
                double a=(y1 - y0);
                zValue=(i/a)/z0+(1-i/a)/z1;
                if(zValue<buffer[(unsigned int) round(x0 + (i / m))][y0 + i] or buffer[(unsigned int) round(x0 + (i / m))][y0 + i]==std::numeric_limits<double>::infinity()) {
                    buffer[(unsigned int) round(x0 + (i / m))][y0 + i] = zValue;
                    image((unsigned int) round(x0 + (i / m)), y0 + i) = color;
                }
            }
        }
        else if (m < -1.0)
        {
            for (unsigned int i = 0; i <= (y0 - y1); i++)
            {
                double a=(y0 - y1);
                zValue=(i/a)/z0+(1-i/a)/z1;
                if(zValue<buffer[(unsigned int) round(x0 - (i / m))][y0 - i] or buffer[(unsigned int) round(x0 - (i / m))][y0 - i]==std::numeric_limits<double>::infinity()) {
                    buffer[(unsigned int) round(x0 - (i / m))][y0 - i] = zValue;
                    image((unsigned int) round(x0 - (i / m)), y0 - i) = color;
                }
            }
        }
    }
}

void ZBuffer::draw_zbuf_triangle(ZBuffer &buffer, img::EasyImage &image, Vector3D &A, Vector3D &B, Vector3D &C, double d,double dx, double dy,Color ambientReflection, Color diffuseReflection, Color specularReflection,double reflectionCoeff, Lights3D &lights,InfLights3D &infLichten) {
    Color kleur= calculateAmbient(ambientReflection,lights);
    std::map<int,std::pair<int,int>> lineCoords;
    Point2D A2D=projection4Triangle(A,d,dx,dy);
    Point2D B2D=projection4Triangle(B,d,dx,dy);
    Point2D C2D=projection4Triangle(C,d,dx,dy);
    int yMax= std::lround(std::max(std::max(A2D.y,B2D.y),C2D.y)-0.5);
    int yMin=std::lround(std::min(std::min(A2D.y,B2D.y),C2D.y)+0.5);
    for(int i=yMin;i<=yMax;i++){
        double xRab=-std::numeric_limits<double>::infinity();
        double xLab=std::numeric_limits<double>::infinity();
        double xRac=-std::numeric_limits<double>::infinity();
        double xLac=std::numeric_limits<double>::infinity();
        double xRbc=-std::numeric_limits<double>::infinity();
        double xLbc=std::numeric_limits<double>::infinity();
        Point2D P=A2D;
        Point2D Q=B2D;
        if((i-P.y)*(i-Q.y)<=0 && P.y!=Q.y) {
            xRab = Q.x + (P.x - Q.x) * (i - Q.y) / (P.y - Q.y);
            xLab=xRab;
        }
        Q=C2D;
        if((i-P.y)*(i-Q.y)<=0 && P.y!=Q.y) {
            xRac = Q.x + (P.x - Q.x) * (i - Q.y) / (P.y - Q.y);
            xLac=xRac;
        }
        P=B2D;
        if((i-P.y)*(i-Q.y)<=0 && P.y!=Q.y) {
            xRbc = Q.x + (P.x - Q.x) * (i - Q.y) / (P.y - Q.y);
            xLbc=xRbc;
        }
        double xL=std::lround(std::min(std::min(xLab,xLac),xLbc)+0.5);
        double xR=std::lround(std::max(std::max(xRab,xRac),xRbc)-0.5);
        lineCoords[i]={xL,xR};
    }
    Point2D zwaartepunt;
    zwaartepunt.x=(A2D.x+B2D.x+C2D.x)/3;
    zwaartepunt.y=(A2D.y+B2D.y+C2D.y)/3;
    double Zzp=1/(3*A.z)+1/(3*B.z)+1/(3*C.z);
    Vector3D U=B-A;
    Vector3D V=C-A;
    Vector3D W;
    W.x=U.y*V.z-U.z*V.y;
    W.y=U.z*V.x-U.x*V.z;
    W.z=U.x*V.y-U.y*V.x;
    for(auto light:infLichten){
        Color diffKleur= calculateDiffuse(diffuseReflection,infLichten,W);
        kleur=kleur+diffKleur;
    }
    double k=W.x*A.x+W.y*A.y+W.z*A.z;
    double dzdx=W.x/(-d*k);
    double dzdy=W.y/(-d*k);
    kleur=limitKleur(kleur);
    img::Color colour=teken2D::fixColor(kleur.red,kleur.green,kleur.blue);
    for(int i=yMin;i<=yMax;i++){
        for(int j=lineCoords[i].first;j<=lineCoords[i].second;j++){
            double zWaarde=1.0001*Zzp+(j-zwaartepunt.x)*dzdx+(i-zwaartepunt.y)*dzdy;
            if(zWaarde<buffer[j][i]){
                buffer[j][i]=zWaarde;
                image(j,i)=colour;
            }
        }
    }
}

Point2D ZBuffer::projection4Triangle(Vector3D &punt,const double &d,const double &dx, const double &dy) {
    Point2D punt2D;
    punt2D.x=(d*punt.x/-punt.z)+dx;
    punt2D.y=(d*punt.y/-punt.z)+dy;
    return punt2D;
}

Color ZBuffer::calculateAmbient(const Color& ambientReflection, const Lights3D& lichten) {
    Color kleur;
    for(const auto& licht:lichten){
        kleur.red += (ambientReflection.red * licht.ambientLight.red);
        kleur.green += (ambientReflection.green * licht.ambientLight.green);
        kleur.blue += (ambientReflection.blue * licht.ambientLight.blue);
    }
    return kleur;
}

Color ZBuffer::calculateDiffuse(const Color &diffuseReflection, const InfLights3D &infLichten,Vector3D W) {
    Color kleur;
    W=Vector3D::normalise(W);
    for(const auto& licht:infLichten){
        Vector3D l=Vector3D::normalise(-licht.ldVector);
        double lichtwaarde=W.x*l.x+W.y*l.y+W.z*l.z;
        if(lichtwaarde>0){
            kleur.red+=diffuseReflection.red*licht.diffuseLight.red*lichtwaarde;
            kleur.green+=diffuseReflection.green*licht.diffuseLight.green*lichtwaarde;
            kleur.blue+=diffuseReflection.blue*licht.diffuseLight.blue*lichtwaarde;
        }
    }
    return kleur;
}

Color ZBuffer::limitKleur(Color kleur) {
    if(kleur.red>1){
        kleur.red=1;
    }
    if(kleur.green>1){
        kleur.green=1;
    }
    if(kleur.blue>1){
        kleur.blue=1;
    }
    return kleur;
}
