/* this file contain Common constants and utility functions */
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

/* 获得[0, 1)之间的随机数 */
inline double random_double()
{
    // 同样需要RAND_MAX + 1.0,用1则得到整型，返回值恒为0...
    return rand() / (RAND_MAX + 1.0);
}

/* 获得[min, max)之间的随机数 */
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

/* 限制x范围 */
inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// Common Headers
// 可能导致重复包含所以删去
/*#include "ray.h"*/
/*#include "vec3.h"*/

#endif