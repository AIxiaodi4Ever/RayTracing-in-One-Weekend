#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

using namespace std;

hittable_list random_scene()
{
    hittable_list world;

    auto checker = make_shared<checker_texture>(
        make_shared<constant_texture>(vec3(0.2, 0.3, 0.1)),
        make_shared<constant_texture>(vec3(0.9, 0.9, 0.9)));

    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    int i = 1;
    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(make_shared<constant_texture>(albedo))));
                }
                else if (choose_mat < 0.95)
                {
                    // mental
                    auto albedo = vec3::random(.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(make_shared<sphere>(center, 0.2, make_shared<mental>(albedo, fuzz)));
                }
                else
                {
                    // glass
                    world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }//b
    }//a

    //world.add(make_shared<sphere>(vec3(0, 0.2, 0.6), 0.3, make_shared<lambertian>(vec3(0, 1.0, 0))));
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(make_shared<constant_texture>(vec3(0.4, 0.2, 0.1)))));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<mental>(vec3(1.0, 1.0, 1.0), 0.0)));

    return world;
}

// ȷ��������ɫ
vec3 ray_color(const ray& r, const hittable_list& world, int depth)
{
    hit_record rec;

    // if we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
        return vec3(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        // ���ڽ�����˵�ڲ����䱻��Ϊ�����գ����Է���vec3(0,0,0)
        return vec3(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    // auto t = unit_direction.y() / sqrt(2) + 0.5;
    auto t = (unit_direction.y() + 1) * 0.5;
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
    const int image_width = 1200;
    const int image_height = 600;
    const int samples_per_pixel = 10;
    const int max_depth = 50;
    const auto aspect_ratio = double(image_width) / image_height;

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;

    cout << "P3\n"
         << image_width << " " << image_height << "\n255\n";

    hittable_list world = random_scene();

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    for (int j = image_height - 1; j >= 0; --j)
    {
        cerr << "\rAlready scaning: " << image_height - j << ' ' << flush;
        for (int i = 0; i < image_width; ++i)
        {
            vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / image_width;
                auto v = (j + random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, max_depth);
            }
            color.write_color(cout, samples_per_pixel);
        }
    }
    cerr << "\nDone.\n";
    return 0;
}