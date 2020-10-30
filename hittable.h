/*
 * this file define the father class of any hittable object 
 * and define a real virtual function hit() to provide a common interface
 */

#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"

// 没有下面的类声明，无法编译
class material;

struct hit_record {
    vec3 p;
    vec3 normal;
    double t;
    double u;
    double v;
    bool front_face;
    shared_ptr<material> mat_ptr;

    void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif