//
// Created by miczi on 10.10.16.
//

#ifndef POLYPHASE_SORTING_BY_MICZI_CONE_H
#define POLYPHASE_SORTING_BY_MICZI_CONE_H

#include <lcms.h>
#include <string>
using namespace std;

class Cone {
public:
    float getVolume(){
        //return M_PI * radius * radius * height / 3.0;
        return radius; //DEBUG
    }

    Cone() {
        this->radius = 0;
        this->height = 0;
    }
    Cone(float radius, float height) {
        this->radius = radius;
        this->height = height;
    }

    float radius;
    float height;

    bool operator> (const Cone &right)
    {
        return (radius > right.radius);
    }
    bool operator< (const Cone &right)
    {
        return (radius < right.radius);
    }
};


#endif //POLYPHASE_SORTING_BY_MICZI_CONE_H
