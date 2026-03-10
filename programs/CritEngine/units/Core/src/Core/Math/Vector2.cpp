#include "Vector2.h"

#include <math.h>
#include <algorithm>

namespace Engine
{
    Vector2 Vector2::add(const Vector2& first, const Vector2& second)
    {
        return Vector2(first.x + second.x, first.y + second.y);
    }

    Vector2 Vector2::sub(const Vector2& first, const Vector2& second)
    {
        return Vector2(first.x - second.x, first.y - second.y);
    }

    Vector2 Vector2::mul(const Vector2& first, const Vector2& second)
    {
        return Vector2(first.x * second.x, first.y * second.y);
    }

    Vector2 Vector2::div(const Vector2& first, const Vector2& second)
    {
        return Vector2(first.x / second.x, first.y / second.y);
    }

    float Vector2::dot(const Vector2& first, const Vector2& second)
    {
        return first.x * second.x + first.y * second.y;
    }

    Vector2 Vector2::add(const Vector2& base, float scalar)
    {
        return Vector2(base.x + scalar, base.y + scalar);
    }

    Vector2 Vector2::sub(const Vector2& base, float scalar)
    {
        return Vector2(base.x - scalar, base.y - scalar);
    }

    Vector2 Vector2::mul(const Vector2& base, float scalar)
    {
        return Vector2(base.x * scalar, base.y * scalar);
    }

    Vector2 Vector2::div(const Vector2& base, float scalar)
    {
        return Vector2(base.x / scalar, base.y / scalar);
    }

    float Vector2::length(const Vector2& base)
    {
        return sqrtf(base.x * base.x + base.y * base.y);
    }

    Vector2 Vector2::normalize(const Vector2& base)
    {
        float maxComponent = std::max({base.x, base.y});
        return Vector2(base.x / maxComponent, base.y / maxComponent);
    }

    float Vector2::angle(const Vector2& first, const Vector2& second)
    {
        float firstLength = sqrtf(first.x * first.x + first.y * first.y);
        float secondLength = sqrtf(second.x * second.x + second.y * second.y);
        return acos((first.x * second.x + first.y * second.y) / (firstLength * secondLength));
    }

    bool Vector2::isEqual(const Vector2& first, const Vector2& second)
    {
        return (first.x == second.x) && (first.y == second.y);
    }

    bool Vector2::isNearlyEqual(const Vector2& first, const Vector2& second, float threshold)
    {
        return (std::abs(first.x - second.x) < threshold) && (std::abs(first.y - second.y) < threshold);
    }
}
