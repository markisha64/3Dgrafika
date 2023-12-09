#pragma once
#include "geometry.h"
#include "material.h"
#include "ray.h"
#include <algorithm>
#include <cmath>
#include <vector>

class Object {
public:
  Material material;
  virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;
};

inline float sgn(float val) { return (0 < val) - (val < 0); }

class Sphere : public Object {
  float r;
  Vec3f c;

public:
  Sphere(const Vec3f &c, const float &r) : c(c), r(r) {}
  Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r) {
    Object::material = mat;
  }

  bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const {
    Vec3f e_minus_c = ray.origin - c;
    float d2 = ray.direction * ray.direction;

    float disc = pow(ray.direction * e_minus_c, 2) -
                 d2 * (e_minus_c * e_minus_c - r * r);

    if (disc < 0) {
      return false;
    } else {
      bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
      if (ray_inside_sphere) {
        float t1 = (-ray.direction * e_minus_c + sqrt(disc)) / d2;
        t = t1;
      } else {
        float t2 = (-ray.direction * e_minus_c - sqrt(disc)) / d2;
        t = t2;
      }

      Vec3f hit_point = ray.origin + ray.direction * t;
      normal = (hit_point - c).normalize();
      return true;
    }
  }
};

class Cuboid : public Object {
  Vec3f min, max;

public:
  Cuboid(const Vec3f &mn, const Vec3f &mx) : min(mn), max(mx) {}
  Cuboid(const Vec3f &mn, const Vec3f &mx, const Material &mat)
      : min(mn), max(mx) {
    Object::material = mat;
  }

  bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const {
    float best_t = std::numeric_limits<float>::max();
    bool result;

    Vec3f a1 = min, b1 = min, c1 = min;

    a1.x = max.x;
    b1.y = max.y;
    c1.z = max.z;

    Vec3f a2 = max, b2 = max, c2 = max;

    a2.x = min.x;
    b2.y = min.y;
    c2.z = min.z;

    Vec3f na1 = Vec3f(0, 0, 1) * sgn(min.z - max.z);
    Vec3f nb1 = Vec3f(1, 0, 0) * sgn(min.x - max.x);
    Vec3f nc1 = Vec3f(0, 1, 0) * sgn(min.y - max.y);

    float lna1 = ray.direction * na1;
    float lnb1 = ray.direction * nb1;
    float lnc1 = ray.direction * nc1;

    Vec3f na2 = Vec3f(0, 0, 1) * sgn(max.z - min.z);
    Vec3f nb2 = Vec3f(1, 0, 0) * sgn(max.x - min.x);
    Vec3f nc2 = Vec3f(0, 1, 0) * sgn(max.y - min.y);

    float lna2 = ray.direction * na2;
    float lnb2 = ray.direction * nb2;
    float lnc2 = ray.direction * nc2;

    // ako smo paralelni s bilo kojom stranicom
    // ne sijecemo kvadar
    if (lna1 == 0)
      return false;
    if (lnb1 == 0)
      return false;
    if (lna1 == 0)
      return false;
    if (lna2 == 0)
      return false;
    if (lnb2 == 0)
      return false;
    if (lna2 == 0)
      return false;

    {
      float da1 = ((min - ray.origin) * na1) / lna1;
      Vec3f p = ray.origin + ray.direction * da1;

      if (std::min(a1.x, b1.x) <= p.x && p.x <= std::max(a1.x, b1.x) &&
          std::min(a1.y, b1.y) <= p.y && p.y <= std::max(a1.y, b1.y)) {
        if (da1 < best_t) {
          result = true;
          best_t = da1;
          t = da1;
          normal = na1;
        } 
      }
    }

    {
      float db1 = ((min - ray.origin) * nb1) / lnb1;
      Vec3f p = ray.origin + ray.direction * db1;

      if (std::min(b1.y, c1.y) <= p.y && p.y <= std::max(b1.y, c1.y) &&
          std::min(b1.z, c1.z) <= p.z && p.z <= std::max(b1.z, c1.z)) {
        if (db1 < best_t) {
          result = true;
          best_t = db1;
          t = db1;
          normal = nb1;
        } 
      }
    }

    {
      float dc1 = ((min - ray.origin) * nc1) / lnc1;
      Vec3f p = ray.origin + ray.direction * dc1;

      if (std::min(c1.x, a1.x) <= p.x && p.x <= std::max(c1.x, a1.x) &&
          std::min(c1.z, a1.z) <= p.z && p.z <= std::max(c1.z, a1.z)) {
        if (dc1 < best_t) {
          result = true;
          best_t = dc1;
          t = dc1;
          normal = nc1;
        } 
      }
    }

    {
      float da2 = ((max - ray.origin) * na2) / lna2;
      Vec3f p = ray.origin + ray.direction * da2;

      if (std::min(a2.x, b2.x) <= p.x && p.x <= std::max(a2.x, b2.x) &&
          std::min(a2.y, b2.y) <= p.y && p.y <= std::max(a2.y, b2.y)) {
        if (da2 < best_t) {
          result = true;
          best_t = da2;
          t = da2;
          normal = na2;
        } 
      }
    }

    {
      float db2 = ((max - ray.origin) * nb2) / lnb2;
      Vec3f p = ray.origin + ray.direction * db2;

      if (std::min(b2.y, c2.y) <= p.y && p.y <= std::max(b2.y, c2.y) &&
          std::min(b2.z, c2.z) <= p.z && p.z <= std::max(b2.z, c2.z)) {
        if (db2 < best_t) {
          result = true;
          best_t = db2;
          t = db2;
          normal = nb2;
        } 
      }
    }

    {
      float dc2 = ((max - ray.origin) * nc2) / lnc2;
      Vec3f p = ray.origin + ray.direction * dc2;

      if (std::min(c2.x, a2.x) <= p.x && p.x <= std::max(c2.x, a2.x) &&
          std::min(c2.z, a2.z) <= p.z && p.z <= std::max(c2.z, a2.z)) {
        if (dc2 < best_t) {
          result = true;
          best_t = dc2;
          t = dc2;
          normal = nc2;
        } 
      }
    }

    return result;
  }
};
