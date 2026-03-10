#pragma once

#include "../Core/Base.h"

namespace Engine
{
    struct ENGINE_API Vector4
    {
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0)
        {
        }

        Vector4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar)
        {
        }

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
        {
        }

        //Static Math Operations
        static Vector4 add(const Vector4& first, const Vector4& second);
        static Vector4 sub(const Vector4& first, const Vector4& second);
        static Vector4 mul(const Vector4& first, const Vector4& second);
        static Vector4 div(const Vector4& first, const Vector4& second);
        static float dot(const Vector4& first, const Vector4& second);

        static Vector4 add(const Vector4& base, float scalar);
        static Vector4 sub(const Vector4& base, float scalar);
        static Vector4 mul(const Vector4& base, float scalar);
        static Vector4 div(const Vector4& base, float scalar);

        static float length(const Vector4& base);
        static Vector4 normalize(const Vector4& base);

        static float angle(const Vector4& first, const Vector4& second);

        static bool isEqual(const Vector4& first, const Vector4& second);
        static bool isNearlyEqual(const Vector4& first, const Vector4& second, float threshold);

        //Object Math Operations
        Vector4 add(const Vector4& other) const { return add(*this, other); }
        Vector4 sub(const Vector4& other) const { return sub(*this, other); }
        Vector4 mul(const Vector4& other) const { return mul(*this, other); }
        Vector4 div(const Vector4& other) const { return div(*this, other); }
        float dot(const Vector4& other) const { return dot(*this, other); }

        Vector4 add(const float scalar) const { return add(*this, scalar); }
        Vector4 sub(const float scalar) const { return sub(*this, scalar); }
        Vector4 mul(const float scalar) const { return mul(*this, scalar); }
        Vector4 div(const float scalar) const { return div(*this, scalar); }

        float length() { return length(*this); }
        Vector4 normalize() { return normalize(*this); }

        float angle(const Vector4& other) { return angle(*this, other); }

        bool isEqual(const Vector4& other) { return isEqual(*this, other); }

        bool isNearlyEqual(const Vector4& other, const float threshold)
        {
            return isNearlyEqual(*this, other, threshold);
        }

        // Operators

        Vector4 operator+(const Vector4& other) const { return add(*this, other); }
        Vector4 operator-(const Vector4& other) const { return sub(*this, other); }
        Vector4 operator*(const Vector4& other) const { return mul(*this, other); }
        Vector4 operator/(const Vector4& other) const { return div(*this, other); }

        Vector4 operator+(const float rhs) const { return add(*this, rhs); }
        Vector4 operator-(const float rhs) const { return sub(*this, rhs); }
        Vector4 operator*(const float rhs) const { return mul(*this, rhs); }
        Vector4 operator/(const float rhs) const { return div(*this, rhs); }

        friend Vector4 operator+(const float& lhs, const Vector4& rhs) { return add(rhs, lhs); }
        friend Vector4 operator-(const float& lhs, const Vector4& rhs) { return sub(rhs, lhs); }
        friend Vector4 operator*(const float& lhs, const Vector4& rhs) { return mul(rhs, lhs); }
        friend Vector4 operator/(const float& lhs, const Vector4& rhs) { return div(rhs, lhs); }
    };
}
