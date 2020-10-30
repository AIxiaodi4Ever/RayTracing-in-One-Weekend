#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

using namespace std;

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

// ���������طֲ��ı���
class lambertian : public material {
public:
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    //vec3 albedo;
    shared_ptr<texture> albedo;
};

// ��������ʽ��Christophe Schlick���ƣ���ò�ͬ������µķ�����
double schlick(double cosine, double ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 * (1 - r0) * pow(1 - cosine, 5);
}

// ��������
class mental : public material {
public:
    mental(const vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray &r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        // dir���ܲ��ǵ�λ���������Ե���unit_vector()
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // ������scattered������scatter......
        // ray scatter = ray(rec.p, reflected);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);        // ��Ϊrec.normal�������������һ�࣬������Զ����true
    }

public:
    vec3 albedo;        // �����ʣ�����RGB�����ֱ𱻷������
    double fuzz;        // ͨ��΢С�ı�������䷽�����ı����ӽ����ֲڶ�
};

// ����ʣ�������ˮ�����ʯ�ȣ�
class dielectric : public material {
public:
    dielectric(double ri) : ref_idx(ri) {}

    virtual bool scatter(const ray&r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        attenuation = vec3(1.0, 1.0, 1.0);  // ��˥��
        double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;

        vec3 unit_direction = unit_vector(r_in.direction());
        // ��˹�����ж��Ƿ�����
        double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if (etai_over_etat * sin_theta > 1.0)     // reflect
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        double reflect_prob = schlick(cos_theta, ref_idx);
        // �����С�ڷ�����ʱ����
        if (random_double() < reflect_prob)
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }

        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }
public:
    double ref_idx;
};

#endif