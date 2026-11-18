#include "vec3.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

int main() {
    hittable_list world;
    std::shared_ptr<material> ground_material =
        std::make_shared<lambertian>(colour{0.5,0.5,0.5});
    world.add(std::make_shared<sphere>(point3{0,-1000,0}, 1000, ground_material));
    for (int i = -11; i < 11; ++i) {
        for (int j = -11; j < 11; ++j) {
            double choose_mat = random_double();
            point3 center{i + 0.9 * random_double(), 0.2, j + 0.9 * random_double()};
            if ((center - point3{4, 0.2, 0}).length() > 0.9) {
                std::shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    colour albedo = colour::random() * colour::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    point3 center2 = center + vec3{0, random_double(0,.5), 0};
                    world.add(std::make_shared<sphere>(center, center2, 0.2,
                                sphere_material));
                } else if (choose_mat < 0.95) {
                    colour albedo = colour::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    std::shared_ptr<material> material1 = std::make_shared<dielectric>(1.5);
    std::shared_ptr<material> material2 =
        std::make_shared<lambertian>(colour{0.4,0.2,0.1});
    std::shared_ptr<material> material3 =
        std::make_shared<metal>(colour{0.7,0.6,0.5}, 0);
    world.add(std::make_shared<sphere>(point3{0,1,0}, 1, material1));
    world.add(std::make_shared<sphere>(point3{-4,1,0}, 1, material2));
    world.add(std::make_shared<sphere>(point3{4,1,0}, 1, material3));
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 5;
    cam.vfov = 20;
    cam.lookfrom = point3{13,2,3};
    cam.lookat = point3{0,0,0};
    cam.vup = vec3{0,1,0};
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10;
    cam.render(world);
    return 0;
}

