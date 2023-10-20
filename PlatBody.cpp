//
// Created by student on 25.03.22.
//

#include "PlatBody.h"

Figure PlatBody::generateCube() {
    Figure Cube;
    Vector3D punt1= Vector3D::point(1,-1,-1);
    Vector3D punt2= Vector3D::point(-1,1,-1);
    Vector3D punt3= Vector3D::point(1,1,1);
    Vector3D punt4= Vector3D::point(-1,-1,1);
    Vector3D punt5= Vector3D::point(1,1,-1);
    Vector3D punt6= Vector3D::point(-1,-1,-1);
    Vector3D punt7= Vector3D::point(1,-1,1);
    Vector3D punt8= Vector3D::point(-1,1,1);
    Cube.points={punt1,punt2,punt3,punt4,punt5,punt6,punt7,punt8};
    Face face1;
    face1.point_indexes={0,4,2,6};
    Face face2;
    face2.point_indexes={4,1,7,2};
    Face face3;
    face3.point_indexes={1,5,3,7};
    Face face4;
    face4.point_indexes={5,0,6,3};
    Face face5;
    face5.point_indexes={6,2,7,3};
    Face face6;
    face6.point_indexes={0,5,1,4};
    Cube.faces={face1,face2,face3,face4,face5,face6};
    return Cube;
}

Figure PlatBody::generateTetrahedron() {
    Figure Tetrahedron;
    Vector3D punt1= Vector3D::point(1,-1,-1);
    Vector3D punt2= Vector3D::point(-1,1,-1);
    Vector3D punt3= Vector3D::point(1,1,1);
    Vector3D punt4= Vector3D::point(-1,-1,1);
    Tetrahedron.points={punt1,punt2,punt3,punt4};
    Face face1;
    face1.point_indexes={0,1,2};
    Face face2;
    face2.point_indexes={1,3,2};
    Face face3;
    face3.point_indexes={0,3,1};
    Face face4;
    face4.point_indexes={0,2,3};
    Tetrahedron.faces={face1,face2,face3,face4};
    return Tetrahedron;
}

Figure PlatBody::generateOctahedron() {
    Figure Octahedron;
    Vector3D punt1= Vector3D::point(1,0,0);
    Vector3D punt2= Vector3D::point(0,1,0);
    Vector3D punt3= Vector3D::point(-1,0,0);
    Vector3D punt4= Vector3D::point(0,-1,0);
    Vector3D punt5= Vector3D::point(0,0,-1);
    Vector3D punt6= Vector3D::point(0,0,1);
    Octahedron.points={punt1,punt2,punt3,punt4,punt5,punt6};
    Face face1;
    face1.point_indexes={0,1,5};
    Face face2;
    face2.point_indexes={1,2,5};
    Face face3;
    face3.point_indexes={2,3,5};
    Face face4;
    face4.point_indexes={3,0,5};
    Face face5;
    face5.point_indexes={1,0,4};
    Face face6;
    face6.point_indexes={2,1,4};
    Face face7;
    face7.point_indexes={3,2,4};
    Face face8;
    face8.point_indexes={0,3,4};
    Octahedron.faces={face1,face2,face3,face4,face5,face6,face7,face8};
    return Octahedron;
}

Figure PlatBody::generateIcosahedron() {
    Figure Icosahedron;
    for(int i=1;i<13;i++){
        if(i==1){
            Icosahedron.points.push_back(Vector3D::point(0,0, sqrt(5)/2));
        }
        else{
            if(i==12){
                Icosahedron.points.push_back(Vector3D::point(0,0,-sqrt(5)/2));
            }
            else{
                if(i<7) {
                    double alpha = (i - 2) * 2 * M_PI / 5;
                    Icosahedron.points.push_back(Vector3D::point(cos(alpha), sin(alpha), 0.5));
                }
                else{
                    double beta = M_PI/5+(i-7)*2*M_PI/5;
                    Icosahedron.points.push_back(Vector3D::point(cos(beta), sin(beta),-0.5));
                }
            }
        }
    }
    Face face1;
    face1.point_indexes={0,1,2};
    Face face2;
    face2.point_indexes={0,2,3};
    Face face3;
    face3.point_indexes={0,3,4};
    Face face4;
    face4.point_indexes={0,4,5};
    Face face5;
    face5.point_indexes={0,5,1};
    Face face6;
    face6.point_indexes={1,6,2};
    Face face7;
    face7.point_indexes={2,6,7};
    Face face8;
    face8.point_indexes={2,7,3};
    Face face9;
    face9.point_indexes={3,7,8};
    Face face10;
    face10.point_indexes={3,8,4};
    Face face11;
    face11.point_indexes={4,8,9};
    Face face12;
    face12.point_indexes={4,9,5};
    Face face13;
    face13.point_indexes={5,9,10};
    Face face14;
    face14.point_indexes={5,10,1};
    Face face15;
    face15.point_indexes={1,10,6};
    Face face16;
    face16.point_indexes={11,7,6};
    Face face17;
    face17.point_indexes={11,8,7};
    Face face18;
    face18.point_indexes={11,9,8};
    Face face19;
    face19.point_indexes={11,10,9};
    Face face20;
    face20.point_indexes={11,6,10};
    Icosahedron.faces={face1,face2,face3,face4,face5,face6,face7,face8,face9,face10,face11,face12,face13,face14,face15,face16,face17,face18,face19,face20};
    return Icosahedron;
}

Figure PlatBody::generateDodecahedron() {
    Figure Dodecahedron;
    Figure Ico=generateIcosahedron();
    std::vector<Vector3D> punten;
    for(int i=0;i<Ico.faces.size();i++){
        std::vector<int> tripoints=Ico.faces[i].point_indexes;
        Vector3D punt1=Ico.points[tripoints[0]];
        Vector3D punt2=Ico.points[tripoints[1]];
        Vector3D punt3=Ico.points[tripoints[2]];
        double x=(punt1.x+punt2.x+punt3.x)/3;
        double y=(punt1.y+punt2.y+punt3.y)/3;
        double z=(punt1.z+punt2.z+punt3.z)/3;
        Vector3D dodecapunt=Vector3D::point(x,y,z);
        punten.push_back(dodecapunt);
    };
    Dodecahedron.points=punten;
    Face face1;
    face1.point_indexes={0,1,2,3,4};
    Face face2;
    face2.point_indexes={0,5,6,7,1};
    Face face3;
    face3.point_indexes={1,7,8,9,2};
    Face face4;
    face4.point_indexes={2,9,10,11,3};
    Face face5;
    face5.point_indexes={3,11,12,13,4};
    Face face6;
    face6.point_indexes={4,13,14,5,0};
    Face face7;
    face7.point_indexes={19,18,17,16,15};
    Face face8;
    face8.point_indexes={19,14,13,12,18};
    Face face9;
    face9.point_indexes={18,12,11,10,17};
    Face face10;
    face10.point_indexes={17,10,9,8,16};
    Face face11;
    face11.point_indexes={16,8,7,6,15};
    Face face12;
    face12.point_indexes={15,6,5,14,19};
    Dodecahedron.faces={face1,face2,face3,face4,face5,face6,face7,face8,face9,face10,face11,face12};
    return Dodecahedron;
}

Figure PlatBody::generateSphere(const double radius, const int n) {
    Figure Cirkel;
    Figure Ico = generateIcosahedron();
    Cirkel.points = Ico.points;
    Cirkel.faces = Ico.faces;
    std::vector<Face> newTriangles;
    for(int i=0;i<n;i++) {
        int oldSize=Cirkel.faces.size();
        for (int j=0;j<oldSize;j++) {
            splitTriangle(Cirkel,newTriangles, Cirkel.faces[j]);
        }
        Cirkel.faces=newTriangles;
        newTriangles={};
    }
    for(auto &punt:Cirkel.points){
        punt.normalise();
        punt*=radius;
    }

    return Cirkel;
}

void PlatBody::splitTriangle(Figure &Cirkel,std::vector<Face> &newTriangles,Face &vlak) {
    Vector3D A=Cirkel.points[vlak.point_indexes[0]];
    Vector3D B=Cirkel.points[vlak.point_indexes[1]];
    Vector3D C=Cirkel.points[vlak.point_indexes[2]];
    Vector3D D=Vector3D::point((A.x+B.x)/2,(A.y+B.y)/2,(A.z+B.z)/2);
    Vector3D E=Vector3D::point((A.x+C.x)/2,(A.y+C.y)/2,(A.z+C.z)/2);
    Vector3D F=Vector3D::point((B.x+C.x)/2,(B.y+C.y)/2,(B.z+C.z)/2);
    Cirkel.points.push_back(D);
    Cirkel.points.push_back(E);
    Cirkel.points.push_back(F);
    int size=Cirkel.points.size();
    Face ADE;
    ADE.point_indexes={vlak.point_indexes[0],size-3,size-2};
    Face BFD;
    BFD.point_indexes={vlak.point_indexes[1],size-1,size-3};
    Face CEF;
    CEF.point_indexes={vlak.point_indexes[2],size-2,size-1};
    Face DFE;
    DFE.point_indexes={size-3,size-1,size-2};
    newTriangles.push_back(ADE);
    newTriangles.push_back(BFD);
    newTriangles.push_back(CEF);
    newTriangles.push_back(DFE);
}

Figure PlatBody::generateCylinder(const int n, const double h) {
    Figure Cilinder;
    double straal=1;
    for(int i=0;i<2*n;i++) {
        double x = cos(2 * i * M_PI / n);
        double y = sin(2 * i * M_PI / n);
        Vector3D punt=Vector3D::point(x,y,0);
        Vector3D punt2=Vector3D::point(x,y,h);
        Cilinder.points.push_back(punt);
        Cilinder.points.push_back(punt2);

    }
    for(int j=0;j<2*n-1;j+=2){
        Face vlak;
        vlak.point_indexes={j,(j+2)%(2*n),(j+3)%(2*n),(j+1)%(2*n)};
        Cilinder.faces.push_back(vlak);
    }
    Face grondVlak;
    for(int p=0;p<n;p++){
        grondVlak.point_indexes.push_back(2*p);
    }
    //BOVENVLAK WERKT ENKEL DOOR VOLGENDE STUKJE DUS OPPASSEN LATER
    //HARDCODE LAATSTE LIJNTJE DAN
    Face bovenVlak;
    for(int p=1;p<n+1;p++){
        bovenVlak.point_indexes.push_back(2*(p)-1);
    }
    Cilinder.faces.push_back(grondVlak);
    Cilinder.faces.push_back(bovenVlak);




    return Cilinder;
}

Figure PlatBody::generateCone(const int n, const double h) {
    Figure Kegel;
    double straal=1;
    for(int i=0;i<n;i++){
        double x=cos(2*i*M_PI/n);
        double y=sin(2*i*M_PI/n);
        Vector3D punt=Vector3D::point(x,y,0);
        Kegel.points.push_back(punt);
    }
    Vector3D hoogtepunt=Vector3D::point(0,0,h);
    Kegel.points.push_back(hoogtepunt);
    for(int j=0;j<n;j++){
        Face vlak;
        vlak.point_indexes={j,(j+1)%(n),int(Kegel.points.size())-1};
        Kegel.faces.push_back(vlak);
    }
    Face lastVlak;
    for(int p=0;p<n;p++){
        lastVlak.point_indexes.push_back(n-p);
    }
    Kegel.faces.push_back(lastVlak);
    return Kegel;
}

Figure PlatBody::generateTorus(const double R, const double r, const int n, const int m) {
    Figure Torus;
    for(int i=0;i<n;i++){
        Face cirkel;
        double u=2*i*M_PI/n;
        for(int j=0;j<m;j++){
            double v=2*j*M_PI/m;
            double x=(R+r* cos(v))* cos(u);
            double y=(R+r* cos(v))* sin(u);
            double z=r* sin(v);
            Vector3D punt=Vector3D::point(x,y,z);
            Torus.points.push_back(punt);
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            Face vlak;
            vlak.point_indexes={i*m+j,((i+1)%n)*m+j,((i+1)%n)*m+(j+1)%m,i*m+(j+1)%m};
            Torus.faces.push_back(vlak);
        }
    }
    return Torus;
}

void PlatBody::generateFractal(Figure &fig, Figures3D &fractal, int nr_iterations, double scale) {
    Figures3D nieuwFractal;
    fractal.push_back(fig);
    Matrix schaalMatrix=teken3D::matrixScale(1/scale);
    for(int i=0;i<nr_iterations;i++) {
        for(auto &figure:fractal) {
            for (int j = 0; j < figure.points.size(); j++) {
                Figure fractaal = figure;
                teken3D::matrixTransform(fractaal,schaalMatrix);
                Vector3D verplaatsing = figure.points[j] - fractaal.points[j];
                Matrix translatieMatrix = teken3D::matrixTranslate(verplaatsing);
                teken3D::matrixTransform(fractaal, translatieMatrix);
                nieuwFractal.push_back(fractaal);
            }
        }
        fractal=nieuwFractal;
        nieuwFractal={};
    }
}
