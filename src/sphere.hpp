#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "material.hpp"
#include "ray.hpp"

#include <cmath>
#include <memory>

class sphere : public hittable {
public:
    sphere(const point3 &static_center, double radius,
            std::shared_ptr<material> mat) :
        center{static_center, vec3{0,0,0}}, radius{std::fmax(0, radius)},
        mat{mat} {}

    sphere(const point3 &center1, const point3 &center2, double radius,
            std::shared_ptr<material> mat) :
        center{center1, center2 - center1}, radius{std::fmax(0, radius)},
        mat{mat} {}

    virtual bool hit(const ray &r, interval ray_t, hit_record &rec)
        const {
        point3 current_center = center.at(r.time());
        vec3 oc = current_center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - radius * radius;
        double discriminant = h*h - a*c;
        if (discriminant < 0) {
            return false;
        }
        double sqrtd = std::sqrt(discriminant);
        double root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        return true;
    }

private:
    ray center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif

