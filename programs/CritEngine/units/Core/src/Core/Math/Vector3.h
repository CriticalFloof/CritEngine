#pragma once

#include "../Core/Base.h"

namespace Engine
{
    struct ENGINE_API Vector3
    {
        float x, y, z;

        Vector3() : x(0), y(0), z(0)
        {
        }

        Vector3(float scalar) : x(scalar), y(scalar), z(scalar)
        {
        }

        Vector3(float x, float y, float z) : x(x), y(y), z(z)
        {
        }

        //Static Math Operations
        static Vector3 add(const Vector3& first, const Vector3& second);
        static Vector3 sub(const Vector3& first, const Vector3& second);
        static Vector3 mul(const Vector3& first, const Vector3& second);
        static Vector3 div(const Vector3& first, const Vector3& second);
        static Vector3 cross(const Vector3& first, const Vector3& second);
        static float dot(const Vector3& first, const Vector3& second);


        static Vector3 add(const Vector3& base, float scalar);
        static Vector3 sub(const Vector3& base, float scalar);
        static Vector3 mul(const Vector3& base, float scalar);
        static Vector3 div(const Vector3& base, float scalar);

        static float length(const Vector3& base);
        static Vector3 normalize(const Vector3& base);

        static float angle(const Vector3& first, const Vector3& second);

        static bool isEqual(const Vector3& first, const Vector3& second);
        static bool isNearlyEqual(const Vector3& first, const Vector3& second, float threshold);

        //Object Math Operations
        Vector3 add(const Vector3& other) const { return add(*this, other); }
        Vector3 sub(const Vector3& other) const { return sub(*this, other); }
        Vector3 mul(const Vector3& other) const { return mul(*this, other); }
        Vector3 div(const Vector3& other) const { return div(*this, other); }
        Vector3 cross(const Vector3& other) const { return cross(*this, other); }
        float dot(const Vector3& other) const { return dot(*this, other); }

        Vector3 add(const float scalar) const { return add(*this, scalar); }
        Vector3 sub(const float scalar) const { return sub(*this, scalar); }
        Vector3 mul(const float scalar) const { return mul(*this, scalar); }
        Vector3 div(const float scalar) const { return div(*this, scalar); }

        float length() const { return length(*this); }
        Vector3 normalize() const { return normalize(*this); }

        float angle(const Vector3& other) const { return angle(*this, other); }

        bool isEqual(const Vector3& other) const { return isEqual(*this, other); }

        bool isNearlyEqual(const Vector3& other, const float threshold) const
        {
            return isNearlyEqual(*this, other, threshold);
        }

        //Operators

        Vector3 operator+(const Vector3& other) const { return add(*this, other); }
        Vector3 operator-(const Vector3& other) const { return sub(*this, other); }
        Vector3 operator*(const Vector3& other) const { return mul(*this, other); }
        Vector3 operator/(const Vector3& other) const { return div(*this, other); }

        Vector3 operator+(const float rhs) const { return add(*this, rhs); }
        Vector3 operator-(const float rhs) const { return sub(*this, rhs); }
        Vector3 operator*(const float rhs) const { return mul(*this, rhs); }
        Vector3 operator/(const float rhs) const { return div(*this, rhs); }

        friend Vector3 operator+(const float& lhs, const Vector3& rhs) { return add(rhs, lhs); }
        friend Vector3 operator-(const float& lhs, const Vector3& rhs) { return sub(rhs, lhs); }
        friend Vector3 operator*(const float& lhs, const Vector3& rhs) { return mul(rhs, lhs); }
        friend Vector3 operator/(const float& lhs, const Vector3& rhs) { return div(rhs, lhs); }
    };
}
