#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "hittable.hpp"
#include "colour.hpp"

#include <iostream>

class camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;

    void render(const hittable &world) {
        initialize();
        std::cout << "P3\n" << image_width << ' ' << image_height <<
            "\n255\n";
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScalines remaining: " << (image_height - j)
                << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                point3 pixel_center = pixel00_loc + (i * pixel_delta_u)
                    + (j * pixel_delta_v);
                vec3 ray_direction = pixel_center - center;
                ray r{center, ray_direction};
                colour pixel_colour = ray_colour(r, world);
                write_colour(std::cout, pixel_colour);
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
        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height *
            (double(image_width)/image_height);
        center = point3(0,0,0);
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        point3 viewport_upper_left = center
            - vec3(0, 0, focal_length)
            - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left +
            0.5 * (pixel_delta_u + pixel_delta_v);
    }

    colour ray_colour(const ray &r, const hittable &world) const {
        hit_record rec;
        if (world.hit(r, interval{0, infinity}, rec)) {
            return 0.5 * (rec.normal + colour(1,1,1));
        }
        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-a) * colour{1.0,1.0,1.0}
            + a * colour{0.5, 0.7, 1.0};
    }

    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
};

#endif

