#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "colour.hpp"

#include <memory>
#include <cmath>

class texture {
public:
    virtual ~texture() = default;
    virtual colour value(double u, double v, const point3 &p) const = 0;
};

class solid_colour : public texture {
public:
    solid_colour(const colour &albedo) : albedo{albedo} {}

    solid_colour(double red, double green, double blue) :
        solid_colour{colour{red, green, blue}} {}

    virtual colour value(double u, double v, const point3 &p) const override {
        return albedo;
    }

private:
    colour albedo;
};

class checker_texture : public texture {
public:
    checker_texture(double scale, std::shared_ptr<texture> even,
            std::shared_ptr<texture> odd) :
        inv_scale{1.0/scale}, even{even}, odd{odd} {}

    checker_texture(double scale, const colour &c1, const colour &c2) :
        checker_texture{scale, std::make_shared<solid_colour>(c1),
        std::make_shared<solid_colour>(c2)} {}

    virtual colour value(double u, double v, const point3 &p) const override {
        int x = int(std::floor(inv_scale * p.x()));
        int y = int(std::floor(inv_scale * p.y()));
        int z = int(std::floor(inv_scale * p.z()));
        if ((x+y+z) % 2 == 0) {
            return even->value(u,v,p);
        }
        return odd->value(u,v,p);
    }

private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

#endif

