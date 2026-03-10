#pragma once

#include "../Core/Base.h"

namespace Engine
{
    struct ENGINE_API Vector2
    {
        float x, y;

        Vector2() : x(0), y(0)
        {
        };

        Vector2(float scalar) : x(scalar), y(scalar)
        {
        };

        Vector2(float x, float y) : x(x), y(y)
        {
        };

        //Static Math Operations
        static Vector2 add(const Vector2& first, const Vector2& second);
        static Vector2 sub(const Vector2& first, const Vector2& second);
        static Vector2 mul(const Vector2& first, const Vector2& second);
        static Vector2 div(const Vector2& first, const Vector2& second);
        static float dot(const Vector2& first, const Vector2& second);

        static Vector2 add(const Vector2& base, float scalar);
        static Vector2 sub(const Vector2& base, float scalar);
        static Vector2 mul(const Vector2& base, float scalar);
        static Vector2 div(const Vector2& base, float scalar);

        static float length(const Vector2& base);
        static Vector2 normalize(const Vector2& base);

        static float angle(const Vector2& first, const Vector2& second);

        static bool isEqual(const Vector2& first, const Vector2& second);
        static bool isNearlyEqual(const Vector2& first, const Vector2& second, float threshold);

        //Object Math Operations
        Vector2 add(const Vector2& other) const { return add(*this, other); }
        Vector2 sub(const Vector2& other) const { return sub(*this, other); }
        Vector2 mul(const Vector2& other) const { return mul(*this, other); }
        Vector2 div(const Vector2& other) const { return div(*this, other); }
        float dot(const Vector2& other) const { return dot(*this, other); }

        Vector2 add(const float scalar) const { return add(*this, scalar); }
        Vector2 sub(const float scalar) const { return sub(*this, scalar); }
        Vector2 mul(const float scalar) const { return mul(*this, scalar); }
        Vector2 div(const float scalar) const { return div(*this, scalar); }

        float length() { return length(*this); }
        Vector2 normalize() { return normalize(*this); }

        float angle(const Vector2& other) { return angle(*this, other); }

        bool isEqual(const Vector2& other) { return isEqual(*this, other); }

        bool isNearlyEqual(const Vector2& other, const float threshold)
        {
            return isNearlyEqual(*this, other, threshold);
        }

        //Operators

        Vector2 operator+(const Vector2& other) const { return add(*this, other); }
        Vector2 operator-(const Vector2& other) const { return sub(*this, other); }
        Vector2 operator*(const Vector2& other) const { return mul(*this, other); }
        Vector2 operator/(const Vector2& other) const { return div(*this, other); }
        bool operator==(const Vector2& other) const { return isEqual(*this, other); }
        bool operator!=(const Vector2& other) const { return !isEqual(*this, other); }

        Vector2 operator+(const float rhs) const { return add(*this, rhs); }
        Vector2 operator-(const float rhs) const { return sub(*this, rhs); }
        Vector2 operator*(const float rhs) const { return mul(*this, rhs); }
        Vector2 operator/(const float rhs) const { return div(*this, rhs); }

        friend Vector2 operator+(const float& lhs, const Vector2& rhs) { return add(rhs, lhs); }
        friend Vector2 operator-(const float& lhs, const Vector2& rhs) { return sub(rhs, lhs); }
        friend Vector2 operator*(const float& lhs, const Vector2& rhs) { return mul(rhs, lhs); }
        friend Vector2 operator/(const float& lhs, const Vector2& rhs) { return div(rhs, lhs); }
    };
}
