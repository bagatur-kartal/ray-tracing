#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <limits>
#include <cstdlib>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degress) {
    return degress * pi / 180.0;
}

inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    return int(random_double(min, max+1));
}

#endif

