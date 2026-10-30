#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "vec3.hpp"

#include <iostream>

using colour = vec3;

inline void write_colour(
        std::ostream& out,
        const colour& pixel_colour
        ) {
    double r = pixel_colour.x();
    double g = pixel_colour.y();
    double b = pixel_colour.z();
    int ir = int(255.999 * r);
    int ig = int(255.999 * g);
    int ib = int(255.999 * b);
    out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif

