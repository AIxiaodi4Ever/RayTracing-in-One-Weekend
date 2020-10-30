/* this file define 3-dimension vector and it's operational rule */
#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <math.h>
#include "rtweekend.h"

class vec3 {
public:

    vec3() : e{0,0,0} { }
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} { }

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    // 获得一个像素内总颜色的叠加后做除法得到最终的颜色
    void write_color(std::ostream &out, int samples_per_pixel)
    {
        // 如果是 1 / samples_per_pixel,则scale为整型，得到的颜色全部为黑色
        auto scale = 1.0 / samples_per_pixel;
        auto r = sqrt(scale * e[0]);
        auto g = sqrt(scale * e[1]);
        auto b = sqrt(scale * e[2]);
        // wirte the translated [0, 255] value of each color component .
        out << static_cast<int>(255.999 * clamp(r, 0, 0.999)) << ' '
            << static_cast<int>(255.999 * clamp(g, 0, 0.999)) << ' '
            << static_cast<int>(255.999 * clamp(b, 0, 0.999)) << '\n';
    }

    // 返回随机向量
    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

public:
    double e[3];
};

// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline vec3 operator/(const vec3 &v, double t)
{
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v)
{
    return (u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]);
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

// 获得光圈内随机的点
vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

// 获得单位圆内的随机变量（伪兰贝特分布）
inline vec3 random_in_unit_sphere() {
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

// 获得单位圆表面的随机反射向量（真正的兰贝特分布）
inline vec3 random_unit_vector() {
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), r);
}

// 获得半圆内均匀分布的随机反射向量 
inline vec3 random_in_hemisphere(const vec3& normal)
{   // 使用random_unit_vector()得到错误图像，原因未知
    //vec3 in_unit_sphere = random_unit_vector();
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

// 返回镜反射向量
inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

// 获得电解质中的折射向量
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cos_theta = dot(-uv, n);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(1.0 - r_out_perp.length_squared()) * n;
    return r_out_perp + r_out_parallel;
}

#endif
