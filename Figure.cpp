//
// Created by student on 18.03.22.
//

#include "Figure.h"

void Figure::triangulate() {
    std::vector<Face> newFaces={};
    for(Face vlak:faces) {
        int size = vlak.point_indexes.size();
        for (int i = 1; i <= size - 2; i++) {
            Face driehoek;
            driehoek.point_indexes = {vlak.point_indexes[0],vlak.point_indexes[i],vlak.point_indexes[i+1]};
            newFaces.push_back(driehoek);
        }
    }
    faces=newFaces;
}
