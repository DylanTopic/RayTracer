#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h" 

void perlin_ground() {
    hittable_list world;

    // --- CHANGED: Ground now uses Noise Texture ---
    // Scale = 4.0 creates a standard marble pattern size on the ground
    auto pertext_ground = make_shared<noise_texture>(4.0);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext_ground)));
    // ----------------------------------------------

    // 2. Left Sphere: Low Frequency Noise
    auto tex_low = make_shared<noise_texture>(0.5);
    world.add(make_shared<sphere>(point3(-2.2, 1, 0), 1.0, make_shared<lambertian>(tex_low)));

    // 3. Center Sphere: Medium Frequency Noise
    auto tex_med = make_shared<noise_texture>(4.0);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, make_shared<lambertian>(tex_med)));

    // 4. Right Sphere: High Frequency Noise
    auto tex_high = make_shared<noise_texture>(10.0);
    world.add(make_shared<sphere>(point3(2.2, 1, 0), 1.0, make_shared<lambertian>(tex_high)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;

    // Look from slightly higher up to see the ground better
    cam.lookfrom = point3(0, 4, 10);
    cam.lookat = point3(0, 1, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    perlin_ground();
}