#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degress) {
    return degress * pi / 180.0;
}

#endif

