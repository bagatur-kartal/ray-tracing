#include "vec3.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "bvh.hpp"
#include "quad.hpp"

void bouncing_spheres() {
    hittable_list world;
    std::shared_ptr<texture> checker =
        std::make_shared<checker_texture>(0.32, colour(.2,.3,.1), colour(.9,.9,.9));
    std::shared_ptr<material> ground_material = std::make_shared<lambertian>(checker);
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
    world = hittable_list{std::make_shared<bvh_node>(world)};
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 5;
    cam.background = colour(0.7,0.8,1.0);
    cam.vfov = 20;
    cam.lookfrom = point3{13,2,3};
    cam.lookat = point3{0,0,0};
    cam.vup = vec3{0,1,0};
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10;
    cam.render(world);
}

void checkered_spheres() {
    hittable_list world;
    std::shared_ptr<texture> checker =
        std::make_shared<checker_texture>(
                0.32, colour{.2, .3, .1}, colour{.9, .9, .9}
                );
    world.add(
            std::make_shared<sphere>(
                point3{0,-10, 0}, 10, std::make_shared<lambertian>(checker)
                )
            );
    world.add(
            std::make_shared<sphere>(
                point3{0, 10, 0}, 10, std::make_shared<lambertian>(checker)
                )
            );
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = colour(0.7,0.8,1.0);
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void earth() {
    std::shared_ptr<texture> earth_texture =
        std::make_shared<image_texture>("res/earthmap.jpg");
    std::shared_ptr<material> earth_surface =
        std::make_shared<lambertian>(earth_texture);
    std::shared_ptr<hittable> globe =
        std::make_shared<sphere>(point3{0,0,0}, 2, earth_surface);
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = colour(0.7,0.8,1.0);
    cam.vfov = 20;
    cam.lookfrom = point3{0,0,12};
    cam.lookat = point3{0,0,0};
    cam.vup = vec3{0,1,0};
    cam.defocus_angle = 0;
    cam.render(hittable_list(globe));
}

void perlin_spheres() {
    hittable_list world;
    std::shared_ptr<texture> pertext = std::make_shared<noise_texture>(4);
    world.add(
            std::make_shared<sphere>(
                point3{0,-1000,0}, 1000, std::make_shared<lambertian>(pertext)
                )
            );
    world.add(
            std::make_shared<sphere>(
                point3{0,2,0}, 2, std::make_shared<lambertian>(pertext)
                )
            );
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = colour(0.7,0.8,1.0);
    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void quads() {
    hittable_list world;
    std::shared_ptr<material> left_red =
        std::make_shared<lambertian>(colour{1.0, 0.2, 0.2});
    std::shared_ptr<material> back_green =
        std::make_shared<lambertian>(colour{0.2, 1.0, 0.2});
    std::shared_ptr<material> right_blue =
        std::make_shared<lambertian>(colour{0.2, 0.2, 1.0});
    std::shared_ptr<material> upper_orange =
        std::make_shared<lambertian>(colour{1.0, 0.5, 0.0});
    std::shared_ptr<material> lower_teal =
        std::make_shared<lambertian>(colour{0.2, 0.8, 0.8});
    world.add(std::make_shared<quad>(
                point3{-3,-2, 5}, vec3{0, 0,-4}, vec3{0, 4, 0}, left_red
                )
            );
    world.add(std::make_shared<quad>(
                point3{-2,-2, 0}, vec3{4, 0, 0}, vec3{0, 4, 0}, back_green
                )
            );
    world.add(std::make_shared<quad>(
                point3{ 3,-2, 1}, vec3{0, 0, 4}, vec3{0, 4, 0}, right_blue
                )
            );
    world.add(std::make_shared<quad>(
                point3{-2, 3, 1}, vec3{4, 0, 0}, vec3{0, 0, 4}, upper_orange
                )
            );
    world.add(std::make_shared<quad>(
                point3{-2,-3, 5}, vec3{4, 0, 0}, vec3{0, 0,-4}, lower_teal
                )
            );
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = colour(0.7,0.8,1.0);
    cam.vfov = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void simple_light() {
    hittable_list world;
    std::shared_ptr<texture> pertext = std::make_shared<noise_texture>(4);
    world.add(
            std::make_shared<sphere>(
                point3{0,-1000,0}, 1000, std::make_shared<lambertian>(pertext)
                )
            );
    world.add(
            std::make_shared<sphere>(
                point3{0,2,0}, 2, std::make_shared<lambertian>(pertext)
                )
            );
    std::shared_ptr<material> difflight =
        std::make_shared<diffuse_light>(colour{4,4,4});
    world.add(
            std::make_shared<quad>(
                point3{3,1,-2}, vec3{2,0,0}, vec3{0,2,0}, difflight
                )
            );
    world.add(
            std::make_shared<sphere>(
                point3{0,7,0}, 2, difflight
                )
            );
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = colour{0,0,0};
    cam.vfov = 20;
    cam.lookfrom = point3{26,3,6};
    cam.lookat = point3{0,2,0};
    cam.vup = vec3{0,1,0};
    cam.defocus_angle = 0;
    cam.render(world);
}

void cornell_box() {
    hittable_list world;
    std::shared_ptr<material> red =
        std::make_shared<lambertian>(colour(.65, .05, .05));
    std::shared_ptr<material> white =
        std::make_shared<lambertian>(colour(.73, .73, .73));
    std::shared_ptr<material> green =
        std::make_shared<lambertian>(colour(.12, .45, .15));
    std::shared_ptr<material> light =
        std::make_shared<diffuse_light>(colour(15, 15, 15));
    world.add(
            std::make_shared<quad>(
                point3{555,0,0}, vec3{0,555,0}, vec3{0,0,555}, green
                )
            );
    world.add(
            std::make_shared<quad>(
                point3{0,0,0}, vec3{0,555,0}, vec3{0,0,555}, red
                )
            );
    world.add(
            std::make_shared<quad>(
                point3{343, 554, 332}, vec3{-130,0,0}, vec3{0,0,-105}, light
                )
            );
    world.add(
            std::make_shared<quad>(
                point3{0,0,0}, vec3{555,0,0}, vec3{0,0,555}, white
                )
            );
    world.add(
            std::make_shared<quad>(
                point3{555,555,555}, vec3{-555,0,0}, vec3{0,0,-555}, white
                )
            );
    world.add(
            std::make_shared<quad>(
                point3{0,0,555}, vec3{555,0,0}, vec3{0,555,0}, white
                )
            );
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.background = colour{0,0,0};
    cam.vfov = 40;
    cam.lookfrom = point3{278, 278, -800};
    cam.lookat= point3{278, 278, 0};
    cam.vup = vec3{0,1,0};
    cam.defocus_angle = 0;
    cam.render(world);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cerr << "Usage: ";
        if (argc < 1) {
            std::cerr << "ray-tracing";
        } else {
            std::cerr << argv[0];
        }
        std::cerr << " scene_index\n";
        return 1;
    }
    int scene = atoi(argv[1]);
    switch (scene) {
    case 0:
        bouncing_spheres();
        break;
    case 1:
        checkered_spheres();
        break;
    case 2:
        earth();
        break;
    case 3:
        perlin_spheres();
        break;
    case 4:
        quads();
        break;
    case 5:
        simple_light();
        break;
    case 6:
        cornell_box();
        break;
    default:
        std::cerr << "Wrong scene index\n";
        break;
    }
    return 0;
}

