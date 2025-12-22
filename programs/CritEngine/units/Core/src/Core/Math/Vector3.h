#pragma once

#include "../Core/Base.h"

namespace Engine {

	struct ENGINE_API Vector3
	{
		float x, y, z;

		Vector3() : x(0), y(0), z(0) {}
		Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		//Static Math Operations
		static Vector3 Add(const Vector3& first, const Vector3& second);
		static Vector3 Sub(const Vector3& first, const Vector3& second);
		static Vector3 Mul(const Vector3& first, const Vector3& second);
		static Vector3 Div(const Vector3& first, const Vector3& second);
		static Vector3 Cross(const Vector3& first, const Vector3& second);
		static float Dot(const Vector3& first, const Vector3& second);
		

		static Vector3 Add(const Vector3& base, const float scalar);
		static Vector3 Sub(const Vector3& base, const float scalar);
		static Vector3 Mul(const Vector3& base, const float scalar);
		static Vector3 Div(const Vector3& base, const float scalar);

		static float Length(const Vector3& base);
		static Vector3 Normalize(const Vector3& base);

		static float Angle(const Vector3& first, const Vector3& second);

		static bool IsEqual(const Vector3& first, const Vector3& second);
		static bool IsNearlyEqual(const Vector3& first, const Vector3& second, const float threshold);

		//Object Math Operations
		Vector3 Add(const Vector3& other) const { return Vector3::Add(*this, other); }
		Vector3 Sub(const Vector3& other) const { return Vector3::Sub(*this, other); }
		Vector3 Mul(const Vector3& other) const { return Vector3::Mul(*this, other); }
		Vector3 Div(const Vector3& other) const { return Vector3::Div(*this, other); }
		Vector3 Cross(const Vector3& other) const { return Vector3::Cross(*this, other); }
		float Dot(const Vector3& other) const { return Vector3::Dot(*this, other); }

		Vector3 Add(const float scalar) const { return Vector3::Add(*this, scalar); }
		Vector3 Sub(const float scalar) const { return Vector3::Sub(*this, scalar); }
		Vector3 Mul(const float scalar) const { return Vector3::Mul(*this, scalar); }
		Vector3 Div(const float scalar) const { return Vector3::Div(*this, scalar); }

		float Length() const { return Vector3::Length(*this); }
		Vector3 Normalize() const { return Vector3::Normalize(*this); }

		float Angle(const Vector3& other) const { return Vector3::Angle(*this, other); }

		bool IsEqual(const Vector3& other) const { return Vector3::IsEqual(*this, other); }
		bool IsNearlyEqual(const Vector3& other, const float threshold) const { return Vector3::IsNearlyEqual(*this, other, threshold); }

		//Operators

		Vector3 operator+(const Vector3& other) const { return Vector3::Add(*this, other); }
		Vector3 operator-(const Vector3& other) const { return Vector3::Sub(*this, other); }
		Vector3 operator*(const Vector3& other) const { return Vector3::Mul(*this, other); }
		Vector3 operator/(const Vector3& other) const { return Vector3::Div(*this, other); }

		Vector3 operator+(const float rhs) const { return Vector3::Add(*this, rhs); }
		Vector3 operator-(const float rhs) const { return Vector3::Sub(*this, rhs); }
		Vector3 operator*(const float rhs) const { return Vector3::Mul(*this, rhs); }
		Vector3 operator/(const float rhs) const { return Vector3::Div(*this, rhs); }

		friend Vector3 operator+(const float& lhs, const Vector3& rhs) { return Vector3::Add(rhs, lhs); }
		friend Vector3 operator-(const float& lhs, const Vector3& rhs) { return Vector3::Sub(rhs, lhs); }
		friend Vector3 operator*(const float& lhs, const Vector3& rhs) { return Vector3::Mul(rhs, lhs); }
		friend Vector3 operator/(const float& lhs, const Vector3& rhs) { return Vector3::Div(rhs, lhs); }
	};

}