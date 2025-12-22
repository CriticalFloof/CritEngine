#include "Vector2.h"

#include <math.h>
#include <algorithm>

namespace Engine {

	Vector2 Vector2::Add(const Vector2& first, const Vector2& second)
	{
		return Vector2(first.x + second.x, first.y + second.y);
	}

	Vector2 Vector2::Sub(const Vector2& first, const Vector2& second)
	{
		return Vector2(first.x - second.x, first.y - second.y);
	}

	Vector2 Vector2::Mul(const Vector2& first, const Vector2& second)
	{
		return Vector2(first.x * second.x, first.y * second.y);
	}

	Vector2 Vector2::Div(const Vector2 & first, const Vector2& second)
	{
		return Vector2(first.x / second.x, first.y / second.y);
	}

	float Vector2::Dot(const Vector2& first, const Vector2& second)
	{
		return first.x * second.x + first.y * second.y;
	}

	Vector2 Vector2::Add(const Vector2& base, float scalar)
	{
		return Vector2(base.x + scalar, base.y + scalar);
	}

	Vector2 Vector2::Sub(const Vector2& base, float scalar)
	{
		return Vector2(base.x - scalar, base.y - scalar);
	}

	Vector2 Vector2::Mul(const Vector2& base, float scalar)
	{
		return Vector2(base.x * scalar, base.y * scalar);
	}

	Vector2 Vector2::Div(const Vector2& base, float scalar)
	{
		return Vector2(base.x / scalar, base.y / scalar);
	}

	float Vector2::Length(const Vector2& base)
	{
		return sqrtf(base.x * base.x + base.y * base.y);
	}

	Vector2 Vector2::Normalize(const Vector2& base)
	{
		float maxComponent = std::max({ base.x, base.y });
		return Vector2(base.x / maxComponent, base.y / maxComponent);
	}

	float Vector2::Angle(const Vector2& first, const Vector2& second)
	{
		float firstLength = sqrtf(first.x * first.x + first.y * first.y);
		float secondLength = sqrtf(second.x * second.x + second.y * second.y);
		return acos((first.x * second.x + first.y * second.y) / (firstLength * secondLength));
	}

	bool Vector2::IsEqual(const Vector2& first, const Vector2& second)
	{
		return (first.x == second.x) && (first.y == second.y);
	}

	bool Vector2::IsNearlyEqual(const Vector2& first, const Vector2& second, float threshold)
	{
		return (std::abs(first.x - second.x) < threshold) && (std::abs(first.y - second.y) < threshold);
	}
}