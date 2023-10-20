//
// Created by student on 18.03.22.
//

#include "teken3D.h"

Matrix teken3D::matrixScale(const double scale) {
    Matrix matrix;
    for(int i=1;i<4;i++){
        matrix(i,i)=scale;
    }
//std::cout<<matrix<<std::endl;
    return matrix;
}

Matrix teken3D::matrixRotateX(double angle) {
    Matrix matrix;
    angle=(angle*M_PI)/180;
    for(int i=2;i<4;i++){
        matrix(i,i)=cos(angle);
    }
    if(sin(angle)==0){
        matrix(3,2)=0;
    }
    else{
        matrix(3,2)=-sin(angle);
    }
    matrix(2,3)=sin(angle);
    //std::cout<<matrix<<std::endl;
    return matrix;
}

Matrix teken3D::matrixRotateY(double angle) {
    Matrix matrix;
    angle=(angle*M_PI)/180;
    for(int i=1;i<4;i+=2){
        matrix(i,i)=cos(angle);
    }
    if(sin(angle)==0){
        matrix(1,3)=0;
    }
    else{
        matrix(1,3)=-sin(angle);
    }
    matrix(3,1)=sin(angle);
    //std::cout<<matrix<<std::endl;
    return matrix;;
}

Matrix teken3D::matrixRotateZ(double angle) {
    Matrix matrix;
    angle=(angle*M_PI)/180;
    for(int i=1;i<3;i++){
        matrix(i,i)=cos(angle);
    }
    if(sin(angle)==0){
        matrix(2,1)=0;
    }
    else{
        matrix(2,1)=-sin(angle);
    }
    matrix(1,2)=sin(angle);
    //std::cout<<matrix<<std::endl;
    return matrix;;
}

Matrix teken3D::matrixTranslate(Vector3D &vector) {
    Matrix matrix;
    matrix(4,1)=vector.x;
    matrix(4,2)=vector.y;
    matrix(4,3)=vector.z;
    //std::cout<<matrix<<std::endl;
    return matrix;
}

void teken3D::matrixTransform(Figure &fig, Matrix &m) {
    for(auto &punt:fig.points){
        punt*=m;
    }
}

Matrix teken3D::matrixEye(Vector3D &eyePoint) {
    Matrix matrix;
    double r= sqrt(pow(eyePoint.x,2)+pow(eyePoint.y,2)+pow(eyePoint.z,2));
    double theta=std::atan2(eyePoint.y,eyePoint.x);
    double phi=std::acos(eyePoint.z/r);
    matrix(1,1)=-sin(theta);
    matrix(1,2)=-cos(theta)*cos(phi);
    matrix(1,3)=cos(theta)*sin(phi);
    matrix(2,1)=cos(theta);
    matrix(2,2)=-sin(theta)*cos(phi);
    matrix(2,3)=sin(theta)*sin(phi);
    matrix(3,2)=sin(phi);
    matrix(3,3)=cos(phi);
    matrix(4,3)=-r;
    return matrix;
}

void teken3D::matrixTransformAll(Figures3D &figs, Matrix &m) {
    for(auto &figure:figs){
        matrixTransform(figure,m);
    }
}

Lines2D teken3D::projectionAll(Figures3D &figs) {
    Lines2D lijnen={};
    for(auto &figure:figs){
        for(auto &vlak:figure.faces){
            int size=vlak.point_indexes.size();
            if(size>2){
                Vector3D punt1=figure.points[vlak.point_indexes[size-1]];
                Vector3D punt2=figure.points[vlak.point_indexes[0]];
                Line2D lijn=Line2D(projection(punt1),projection(punt2),figure.color);
                lijnen.push_back(lijn);
            }
            for(int i=1;i<size;i++){ /////////////////
                Vector3D punt1=figure.points[vlak.point_indexes[i-1]];
                Vector3D punt2=figure.points[vlak.point_indexes[i]];
                Line2D lijn=Line2D(projection(punt1),projection(punt2),figure.color);
                lijn.z0=-1/punt1.z;
                lijn.z1=-1/punt2.z;
                lijnen.push_back(lijn);
            }
        }
    }
    return lijnen;
}

Point2D teken3D::projection(Vector3D &point, double d) {
    if(point.z==0){
        Point2D punt=Point2D(point.x,point.y);
        return punt;
    }
    else{
        double X=(d*point.x)/-point.z;
        double Y=(d*point.y)/-point.z;
        Point2D punt=Point2D(X,Y);
        return punt;
    }
}

std::string teken3D::transformString(const LParser::LSystem3D &lsysteem) {
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

Figure teken3D::stringToFigure(const std::string& stringToDraw, const LParser::LSystem3D &lsysteem) {
    Figure figuur;
    double angle=(lsysteem.get_angle()*M_PI)/180;
    std::set<char> alfabet=lsysteem.get_alphabet();
    Vector3D position_current=Vector3D::point(0,0,0);
    Vector3D position_last=Vector3D::point(0,0,0);
    Vector3D H=Vector3D::vector(1,0,0);
    Vector3D L=Vector3D::vector(0,1,0);
    Vector3D U=Vector3D::vector(0,0,1);
    Vector3D Hnew;
    Vector3D Lnew;
    std::vector<Vector3D> savedPos;
    std::vector<Vector3D> savedH;
    std::vector<Vector3D> savedL;
    std::vector<Vector3D> savedU;
    for(auto symbool:stringToDraw){
        if(symbool=='+'){
            Hnew=H*cos(angle)+L* sin(angle);
            L=-H* sin(angle)+L* cos(angle);
            H=Hnew;
        }
        else if(symbool=='-'){
            Hnew=H*cos(-angle)+L* sin(-angle);
            L=-H* sin(-angle)+L* cos(-angle);
            H=Hnew;
        }
        else if(symbool=='^'){
            Hnew=H* cos(angle)+U* sin(angle);
            U=-H* sin(angle)+U* cos(angle);
            H=Hnew;
        }
        else if(symbool=='&'){
            Hnew=H* cos(-angle)+U* sin(-angle);
            U=-H* sin(-angle)+U* cos(-angle);
            H=Hnew;
        }
        else if(symbool=='\\'){
            Lnew=L* cos(angle)-U*sin(angle);
            U=L* sin(angle)+U* cos(angle);
            L=Lnew;
        }
        else if(symbool=='/'){
            Lnew=L* cos(-angle)-U*sin(-angle);
            U=L* sin(-angle)+U* cos(-angle);
            L=Lnew;
        }
        else if(symbool=='|'){
            H=-H;
            L=-L;
        }
        else if(symbool=='('){
            savedPos.push_back(position_current);
            savedH.push_back(H);
            savedL.push_back(L);
            savedU.push_back(U);
        }
        else if(symbool==')'){
            int size=savedPos.size();
            position_current=savedPos[size-1];
            int sizeH=savedH.size();
            H=savedH[sizeH-1];
            int sizeL=savedL.size();
            L=savedL[sizeL-1];
            int sizeU=savedU.size();
            U=savedU[sizeU-1];
            savedPos.pop_back();
            savedH.pop_back();
            savedL.pop_back();
            savedU.pop_back();
        }
        else if(alfabet.find(symbool)!=alfabet.end()){
            position_last=position_current;
            position_current+=H;
            if(lsysteem.draw(symbool)){
                Face lijn3D;
                figuur.points.push_back(position_last);
                figuur.points.push_back(position_current);
                int size=figuur.points.size();
                lijn3D.point_indexes.push_back(size-2);
                lijn3D.point_indexes.push_back(size-1);
                figuur.faces.push_back(lijn3D);
            }
        }
    }
    return figuur;
}
