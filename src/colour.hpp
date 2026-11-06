#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "vec3.hpp"
#include "interval.hpp"

#include <iostream>

using colour = vec3;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

inline void write_colour(
        std::ostream& out,
        const colour& pixel_colour
        ) {
    double r = pixel_colour.x();
    double g = pixel_colour.y();
    double b = pixel_colour.z();
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    static const interval intensity{0.0, 0.999};
    int ir = int(256 * intensity.clamp(r));
    int ig = int(256 * intensity.clamp(g));
    int ib = int(256 * intensity.clamp(b));
    out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif

