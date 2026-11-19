#ifndef QUAD_HPP
#define QUAD_HPP

#include "hittable.hpp"
#include "hittable_list.hpp"

class quad : public hittable {
public:
    quad(const point3 &Q, const vec3 &u, const vec3 &v,
            std::shared_ptr<material> mat) :
        Q{Q}, u{u}, v{v}, mat{mat} {
        vec3 n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n,n);
        set_bounding_box();
    }

    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        double denom = dot(normal, r.direction());
        if (std::fabs(denom) < 1e-8) {
            return false;
        }
        double t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t)) {
            return false;
        }
        vec3 intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        double alpha = dot(w, cross(planar_hitpt_vector, v));
        double beta = dot(w, cross(u, planar_hitpt_vector));
        if (!is_interior(alpha, beta, rec)) {
            return false;
        }
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);
        return true;
    }

    virtual aabb bounding_box() const override {
        return bbox;
    }

    virtual void set_bounding_box() {
        aabb bbox_diagonal1 = aabb{Q, Q+u+v};
        aabb bbox_diagonal2 = aabb{Q+u, Q+v};
        bbox = aabb{bbox_diagonal1, bbox_diagonal2};
    }

    virtual bool is_interior(double a, double b, hit_record &rec) const {
        interval unit_interval{0, 1};
        if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
            return false;
        }
        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    point3 Q;
    vec3 u,v,w;
    std::shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

inline std::shared_ptr<hittable_list> box(const point3&a, const point3 &b,
        std::shared_ptr<material> mat) {
    std::shared_ptr<hittable_list> sides = std::make_shared<hittable_list>();
    point3 min{
        std::fmin(a.x(), b.x()),
        std::fmin(a.y(), b.y()),
        std::fmin(a.z(), b.z())
    };
    point3 max{
        std::fmax(a.x(), b.x()),
        std::fmax(a.y(), b.y()),
        std::fmax(a.z(), b.z())
    };
    point3 dx{max.x() - min.x(), 0, 0};
    point3 dy{0, max.y() - min.y(), 0};
    point3 dz{0, 0, max.z() - min.z()};
    sides->add(std::make_shared<quad>(
                point3{min.x(), min.y(), max.z()}, dx, dy, mat)
            );
    sides->add(std::make_shared<quad>(
                point3{max.x(), min.y(), max.z()}, -dz, dy, mat)
            );
    sides->add(std::make_shared<quad>(
                point3{max.x(), min.y(), min.z()}, -dx, dy, mat)
            );
    sides->add(std::make_shared<quad>(
                point3{min.x(), min.y(), min.z()}, dz, dy, mat)
            );
    sides->add(std::make_shared<quad>(
                point3{min.x(), max.y(), max.z()}, dx, -dz, mat)
            );
    sides->add(std::make_shared<quad>(
                point3{min.x(), min.y(), min.z()}, dx, dz, mat)
            );
    return sides;
}

#endif

