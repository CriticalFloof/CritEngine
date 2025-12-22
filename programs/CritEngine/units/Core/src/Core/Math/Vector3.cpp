#include "Vector3.h"

#include <math.h>
#include <algorithm>

namespace Engine {

	Vector3 Vector3::Add(const Vector3& first, const Vector3& second)
	{
		return Vector3(first.x + second.x, first.y + second.y, first.z + second.z);
	}

	Vector3 Vector3::Sub(const Vector3& first, const Vector3& second)
	{
		return Vector3(first.x - second.x, first.y - second.y, first.z - second.z);
	}

	Vector3 Vector3::Mul(const Vector3& first, const Vector3& second)
	{
		return Vector3(first.x * second.x, first.y * second.y, first.z * second.z);
	}

	Vector3 Vector3::Div(const Vector3& first, const Vector3& second)
	{
		return Vector3(first.x / second.x, first.y / second.y, first.z / second.z);
	}

	Vector3 Vector3::Cross(const Vector3& first, const Vector3& second)
	{
		return Vector3(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
	}

	float Vector3::Dot(const Vector3& first, const Vector3& second)
	{
		return first.x * second.x + first.y * second.y + first.z * second.z;
	}

	Vector3 Vector3::Add(const Vector3& base, float scalar)
	{
		return Vector3(base.x + scalar, base.y + scalar, base.z + scalar);
	}

	Vector3 Vector3::Sub(const Vector3& base, float scalar)
	{
		return Vector3(base.x - scalar, base.y - scalar, base.z - scalar);
	}

	Vector3 Vector3::Mul(const Vector3& base, float scalar)
	{
		return Vector3(base.x * scalar, base.y * scalar, base.z * scalar);
	}

	Vector3 Vector3::Div(const Vector3& base, float scalar)
	{
		return Vector3(base.x / scalar, base.y / scalar, base.z / scalar);
	}

	float Vector3::Length(const Vector3& base)
	{
		return sqrtf(base.x * base.x + base.y * base.y + base.z * base.z);
	}

	Vector3 Vector3::Normalize(const Vector3& base)
	{
		float maxComponent = std::max({ abs(base.x), abs(base.y), abs(base.z) });
		return Vector3(base.x / maxComponent, base.y / maxComponent, base.z / maxComponent);
	}

	float Vector3::Angle(const Vector3& first, const Vector3& second)
	{
		float firstLength = sqrtf(first.x * first.x + first.y * first.y + first.z * first.z); 
		float secondLength = sqrtf(second.x * second.x + second.y * second.y + second.z * second.z);
		return acos((first.x * second.x + first.y * second.y + first.z * second.z) / (firstLength * secondLength));
	}

	bool Vector3::IsEqual(const Vector3& first, const Vector3& second)
	{
		return (first.x == second.x) && (first.y == second.y) && (first.z == second.z);
	}

	bool Vector3::IsNearlyEqual(const Vector3& first, const Vector3& second, float threshold)
	{
		return (std::abs(first.x - second.x) < threshold) && (std::abs(first.y - second.y) < threshold) && (std::abs(first.z - second.z) < threshold);
	}
}