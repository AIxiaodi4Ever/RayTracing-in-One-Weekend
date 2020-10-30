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

/* ���[0, 1)֮�������� */
inline double random_double()
{
    // ͬ����ҪRAND_MAX + 1.0,��1��õ����ͣ�����ֵ��Ϊ0...
    return rand() / (RAND_MAX + 1.0);
}

/* ���[min, max)֮�������� */
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

/* ����x��Χ */
inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// Common Headers
// ���ܵ����ظ���������ɾȥ
/*#include "ray.h"*/
/*#include "vec3.h"*/

#endif