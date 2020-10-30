/* this file define camera class from camera we can get ray */
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "ray.h"
#include "vec3.h"

class camera {
public:
    // 参数分别是镜头位置、观察点中心位置，vup一般为(0,1,0)用以计算局部坐标系下正交基的单位向量，
    // vfov是竖直方向视线角度范围，aspect是水平视线范围与竖直视线范围的比值，aperture是光圈大小
    // focus_disk是镜头到物品中心点的距离
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspest, double aperture, double focus_disk) 
    {
        origin = lookfrom;
        lens_radius = aperture / 2;

        auto theta = degrees_to_radians(vfov);
        auto half_height = tan(theta / 2);
        auto half_width = aspest * half_height;

        // 改变lookat会影响视线的中心
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w)); // 文档里v的方向画反了
        v = cross(w, u);                // 单位向量相乘一定为单位向量

        // 这里改变任何参数都会影响视线范围（更改w影响缩放，更改uv影响平移）
        // 存在光圈时，不在焦距focus_disk范围内的物体，光线的发射方向无法对准该物体，所以非常模糊
        lower_left_corner = origin - half_width * focus_disk * u - half_height * focus_disk * v - focus_disk * w;       
        horizontal = 2 * half_width * focus_disk * u;
        vertical = 2 * half_height * focus_disk * v;
    }

    ray get_ray(double s, double t)
    {
        // 获得镜头范围内的随机点
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};

#endif