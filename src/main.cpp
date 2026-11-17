#include "vec3.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

int main() {
    hittable_list world;
    std::shared_ptr<material> material_ground =
        std::make_shared<lambertian>(colour{0.8,0.8,0.0});
    std::shared_ptr<material> material_center =
        std::make_shared<lambertian>(colour{0.1,0.2,0.5});
    std::shared_ptr<material> material_left =
        std::make_shared<dielectric>(1.5);
    std::shared_ptr<material> material_bubble =
        std::make_shared<dielectric>(1.0/1.33);
    std::shared_ptr<material> material_right =
        std::make_shared<metal>(colour{0.8,0.6,0.2}, 1.0);
    world.add(std::make_shared<sphere>(point3{0,-100.5,-1}, 100, material_ground));
    world.add(std::make_shared<sphere>(point3{0,0,-1.2}, 0.5, material_center));
    world.add(std::make_shared<sphere>(point3{-1.0,0,-1.0}, 0.5, material_left));
    world.add(std::make_shared<sphere>(point3{-1.0,0,-1.0}, 0.4, material_bubble));
    world.add(std::make_shared<sphere>(point3{1.0,0,-1.0}, 0.5, material_right));
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10000;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookfrom = point3{-2,2,1};
    cam.lookat = point3{0,0,-1};
    cam.vup = vec3{0,1,0};
    cam.defocus_angle = 10;
    cam.focus_dist = 3.4;
    cam.render(world);
    return 0;
}

