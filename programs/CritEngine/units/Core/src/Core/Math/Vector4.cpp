#include "Vector4.h"

#include <math.h>
#include <algorithm>

namespace Engine {

	Vector4 Vector4::Add(const Vector4& first, const Vector4& second)
	{
		return Vector4(first.x + second.x, first.y + second.y, first.z + second.z, first.w + second.w);
	}

	Vector4 Vector4::Sub(const Vector4& first, const Vector4& second)
	{
		return Vector4(first.x - second.x, first.y - second.y, first.z - second.z, first.w - second.w);
	}

	Vector4 Vector4::Mul(const Vector4& first, const Vector4& second)
	{
		return Vector4(first.x * second.x, first.y * second.y, first.z * second.z, first.w * second.w);
	}

	Vector4 Vector4::Div(const Vector4& first, const Vector4& second)
	{
		return Vector4(first.x / second.x, first.y / second.y, first.z / second.z, first.w / second.w);
	}

	float Vector4::Dot(const Vector4& first, const Vector4& second)
	{
		return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
	}

	Vector4 Vector4::Add(const Vector4& base, float scalar)
	{
		return Vector4(base.x + scalar, base.y + scalar, base.z + scalar, base.w + scalar);
	}

	Vector4 Vector4::Sub(const Vector4& base, float scalar)
	{
		return Vector4(base.x - scalar, base.y - scalar, base.z - scalar, base.w - scalar);
	}

	Vector4 Vector4::Mul(const Vector4& base, float scalar)
	{
		return Vector4(base.x * scalar, base.y * scalar, base.z * scalar, base.w * scalar);
	}

	Vector4 Vector4::Div(const Vector4& base, float scalar)
	{
		return Vector4(base.x / scalar, base.y / scalar, base.z / scalar, base.w / scalar);
	}

	float Vector4::Length(const Vector4& base)
	{
		return sqrtf(base.x * base.x + base.y * base.y + base.z * base.z + base.w * base.w);
	}

	Vector4 Vector4::Normalize(const Vector4& base)
	{
		float maxComponent = std::max({ base.x, base.y, base.z, base.w });
		return Vector4(base.x / maxComponent, base.y / maxComponent, base.z / maxComponent, base.w / maxComponent);
	}

	float Vector4::Angle(const Vector4& first, const Vector4& second)
	{
		float firstLength = sqrtf(first.x * first.x + first.y * first.y + first.z * first.z + first.w * first.w);
		float secondLength = sqrtf(second.x * second.x + second.y * second.y + second.z * second.z + second.w * second.w);
		return acos((first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w) / (firstLength * secondLength));
	}

	bool Vector4::IsEqual(const Vector4& first, const Vector4& second)
	{
		return 
			(first.x == second.x) && 
			(first.y == second.y) && 
			(first.z == second.z) && 
			(first.w == second.w);
	}

	bool Vector4::IsNearlyEqual(const Vector4& first, const Vector4& second, float threshold)
	{
		return 
			(std::abs(first.x - second.x) < threshold) && 
			(std::abs(first.y - second.y) < threshold) && 
			(std::abs(first.z - second.z) < threshold) && 
			(std::abs(first.w - second.w) < threshold);
	}
}