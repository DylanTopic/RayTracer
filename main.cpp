#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"
#include "triangle.h" 

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat) {
    auto sides = make_shared<hittable_list>();
    auto min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()), dx, dy, mat));
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz, dy, mat)); 
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx, dy, mat)); 
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dz, dy, mat));
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()), dx, -dz, mat)); 
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dx, dz, mat)); 

    return sides;
}

void final_showcase() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    auto noise = make_shared<noise_texture>(0.2); 
    auto ground_mat = make_shared<lambertian>(checker);
    auto white_mat = make_shared<lambertian>(color(.73, .73, .73));
    auto metal_mat = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto glass_mat = make_shared<dielectric>(1.5);
    auto light_mat = make_shared<diffuse_light>(color(7, 7, 7));
    auto earth_mat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));

    world.add(make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light_mat));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, glass_mat));

    world.add(make_shared<sphere>(point3(0, 150, 145), 50, metal_mat));

    world.add(make_shared<sphere>(point3(400, 200, 100), 100, earth_mat));

    world.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(noise)));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_mat = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, moving_mat));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, glass_mat);
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));

    auto global_boundary = make_shared<sphere>(point3(0, 0, 0), 5000, glass_mat);
    world.add(make_shared<constant_medium>(global_boundary, 0.0001, color(1, 1, 1)));

    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white_mat);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    auto red_mat = make_shared<lambertian>(color(0.8, 0.1, 0.1));
    point3 top(100, 150, 100);
    point3 v1(50, 0, 150);
    point3 v2(150, 0, 150);
    point3 v3(100, 0, 50);

    
    world.add(make_shared<triangle>(v1, v2 - v1, top - v1, red_mat));    
    world.add(make_shared<triangle>(v2, v3 - v2, top - v2, red_mat));
    world.add(make_shared<triangle>(v3, v1 - v3, top - v3, red_mat));

    world = hittable_list(make_shared<bvh_node>(world));

  
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    final_showcase();
}