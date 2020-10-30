/* this file define camera class from camera we can get ray */
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "ray.h"
#include "vec3.h"

class camera {
public:
    // �����ֱ��Ǿ�ͷλ�á��۲������λ�ã�vupһ��Ϊ(0,1,0)���Լ���ֲ�����ϵ���������ĵ�λ������
    // vfov����ֱ�������߽Ƕȷ�Χ��aspect��ˮƽ���߷�Χ����ֱ���߷�Χ�ı�ֵ��aperture�ǹ�Ȧ��С
    // focus_disk�Ǿ�ͷ����Ʒ���ĵ�ľ���
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspest, double aperture, double focus_disk) 
    {
        origin = lookfrom;
        lens_radius = aperture / 2;

        auto theta = degrees_to_radians(vfov);
        auto half_height = tan(theta / 2);
        auto half_width = aspest * half_height;

        // �ı�lookat��Ӱ�����ߵ�����
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w)); // �ĵ���v�ķ��򻭷���
        v = cross(w, u);                // ��λ�������һ��Ϊ��λ����

        // ����ı��κβ�������Ӱ�����߷�Χ������wӰ�����ţ�����uvӰ��ƽ�ƣ�
        // ���ڹ�Ȧʱ�����ڽ���focus_disk��Χ�ڵ����壬���ߵķ��䷽���޷���׼�����壬���Էǳ�ģ��
        lower_left_corner = origin - half_width * focus_disk * u - half_height * focus_disk * v - focus_disk * w;       
        horizontal = 2 * half_width * focus_disk * u;
        vertical = 2 * half_height * focus_disk * v;
    }

    ray get_ray(double s, double t)
    {
        // ��þ�ͷ��Χ�ڵ������
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