#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"
#include "colour.hpp"
#include "material.hpp"

#include <iostream>

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    colour background;
    double vfov = 90;
    point3 lookfrom = point3{0,0,0};
    point3 lookat = point3{0,0,-1};
    vec3 vup = vec3{0,1,0};
    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable &world) {
        initialize();
        std::cout << "P3\n" << image_width << ' ' << image_height <<
            "\n255\n";
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScalines remaining: " << (image_height - j)
                << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                colour pixel_colour{0,0,0};
                for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
                    for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                        ray r = get_ray(i, j, s_i, s_j);
                        pixel_colour += ray_colour(r, max_depth, world);
                    }
                }
                write_colour(std::cout, pixel_samples_scale * pixel_colour);
            }
        }
        std::clog << "\rDone.                     \n";
    }

private:
    void initialize() {
        image_height = int(image_width / aspect_ratio);
        if (image_height < 1) {
            image_height = 1;
        }
        sqrt_spp = std::sqrt(samples_per_pixel);
        pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
        recip_sqrt_spp = 1.0/sqrt_spp;
        center = lookfrom;
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta/2);
        double viewport_height = 2 * h * focus_dist;
        double viewport_width = viewport_height *
            (double(image_width)/image_height);
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        point3 viewport_upper_left = center
            - (focus_dist * w)
            - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left +
            0.5 * (pixel_delta_u + pixel_delta_v);
        double defocus_radius =
            focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    colour ray_colour(const ray &r, int depth, const hittable &world) const {
        if (depth <= 0) {
            return colour{0,0,0};
        }
        hit_record rec;
        if (!world.hit(r, interval{0.001, infinity}, rec)) {
            return background;
        }
        ray scattered;
        colour attenuation;
        colour colour_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);
        if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
            return colour_from_emission;
        }
        colour colour_from_scatter = attenuation
            * ray_colour(scattered, depth-1, world);
        return colour_from_emission + colour_from_scatter;
    }

    ray get_ray(int i, int j, int s_i, int s_j) const {
        vec3 offset = sample_square_stratified(s_i, s_j);
        point3 pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);
        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        vec3 ray_direction = pixel_sample - ray_origin;
        double ray_time = random_double();
        return ray{ray_origin, ray_direction, ray_time};
    }

    vec3 sample_square() const {
        return vec3{random_double() - 0.5, random_double() - 0.5, 0};
    }

    vec3 sample_square_stratified(int s_i, int s_j) const {
        double px = ((s_i + random_double()) * recip_sqrt_spp) - 0.5;
        double py = ((s_j + random_double()) * recip_sqrt_spp) - 0.5;
        return vec3{px, py, 0};
    }

    point3 defocus_disk_sample() const {
        vec3 p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    int image_height;
    double pixel_samples_scale;
    int sqrt_spp;
    double recip_sqrt_spp;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u,v,w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;
};

#endif

