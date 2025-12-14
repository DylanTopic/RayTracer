#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

int main() {
    hittable_list world;
    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, make_shared<lambertian>(checker)));

    auto material_center = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto center1 = point3(0.0, 0.0, -1.0);
    auto center2 = point3(0.0, 0.5, -1.0);
    world.add(make_shared<sphere>(center1, center2, 0.5, material_center));

    auto material_left = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, make_shared<dielectric>(1.00 / 1.50)));

    auto material_right = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0, 2, 6);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 7.28;

    cam.render(world);
}