/* 
 * this file define the ray(eye-ray...) incldue emit-point(orig) and direction(dir) 
 * and a function : vec3 at(double t) const; to get it's state at time t 
 *
 * 
 */
#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include <iostream>

using namespace std;

class ray {
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction) : orig(origin), dir(direction) { }

    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    vec3 at(double t) const {
        return orig + t * dir;
    }

public: 
    vec3 orig;
    vec3 dir;
};

#endif