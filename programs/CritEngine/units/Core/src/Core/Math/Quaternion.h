#pragma once

#include "Vector3.h"
#include "../Core/Base.h"

namespace Engine
{
    struct ENGINE_API Quaternion
    {
        float x, y, z, w;

        Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w)
        {
        }

        static Quaternion fromEulerAngles(Vector3 angles);
        static Quaternion fromAxisAngle(Vector3 axis, float angle);

        // Static Math Operations
        static Quaternion mul(const Quaternion& first, const Quaternion& second);
        static Quaternion log(const Quaternion& base);
        static Quaternion exp(const Quaternion& base);
        static Quaternion conjugate(const Quaternion& base);
        static Quaternion normalize(const Quaternion& base);
        static Quaternion slerpShort(const Quaternion& start, const Quaternion& end, float alpha);
        static Quaternion slerpLong(const Quaternion& start, const Quaternion& end, float alpha);
        // TODO: Move this function to a spline class once we have a need for one.
        static Quaternion squadSpline(const std::vector<Quaternion>& spline, float alpha);
        static Vector3 rotateVector(const Quaternion& base, const Vector3& vector);
        static Vector3 toEulerAngles(const Quaternion& base);

        // Object Math Operations
        Quaternion mul(Quaternion& base) const { return mul(*this, base); }
        Quaternion log() const { return log(*this); }
        Quaternion exp() const { return exp(*this); }
        Quaternion conjugate() const { return conjugate(*this); }
        Quaternion normalize() const { return normalize(*this); }
        Vector3 rotateVector(const Vector3& vector) const { return rotateVector(*this, vector); }
        Vector3 toEulerAngles() const { return toEulerAngles(*this); }

        // Operators

        Quaternion operator*(const Quaternion& rhs) const { return mul(*this, rhs); }
    };
}
