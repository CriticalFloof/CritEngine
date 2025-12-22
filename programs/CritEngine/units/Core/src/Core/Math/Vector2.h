#pragma once

#include "../Core/Base.h"

namespace Engine {

	struct ENGINE_API Vector2
	{
		float x, y;

		Vector2() : x(0), y(0) {};
		Vector2(float scalar) : x(scalar), y(scalar) {};
		Vector2(float x, float y) : x(x), y(y) {};

		//Static Math Operations
		static Vector2 Add(const Vector2& first, const Vector2& second);
		static Vector2 Sub(const Vector2& first, const Vector2& second);
		static Vector2 Mul(const Vector2& first, const Vector2& second);
		static Vector2 Div(const Vector2& first, const Vector2& second);
		static float Dot(const Vector2& first, const Vector2& second);

		static Vector2 Add(const Vector2& base, const float scalar);
		static Vector2 Sub(const Vector2& base, const float scalar);
		static Vector2 Mul(const Vector2& base, const float scalar);
		static Vector2 Div(const Vector2& base, const float scalar);

		static float Length(const Vector2& base);
		static Vector2 Normalize(const Vector2& base);

		static float Angle(const Vector2& first, const Vector2& second);

		static bool IsEqual(const Vector2& first, const Vector2& second);
		static bool IsNearlyEqual(const Vector2& first, const Vector2& second, const float threshold);

		//Object Math Operations
		Vector2 Add(const Vector2& other) const { return Vector2::Add(*this, other); }
		Vector2 Sub(const Vector2& other) const { return Vector2::Sub(*this, other); }
		Vector2 Mul(const Vector2& other) const { return Vector2::Mul(*this, other); }
		Vector2 Div(const Vector2& other) const { return Vector2::Div(*this, other); }
		float Dot(const Vector2& other) const { return Vector2::Dot(*this, other); }

		Vector2 Add(const float scalar) const { return Vector2::Add(*this, scalar); }
		Vector2 Sub(const float scalar) const { return Vector2::Sub(*this, scalar); }
		Vector2 Mul(const float scalar) const { return Vector2::Mul(*this, scalar); }
		Vector2 Div(const float scalar) const { return Vector2::Div(*this, scalar); }

		float Length() { return Vector2::Length(*this); }
		Vector2 Normalize() { return Vector2::Normalize(*this); }

		float Angle(const Vector2& other) { return Vector2::Angle(*this, other); }

		bool IsEqual(const Vector2& other) { return Vector2::IsEqual(*this, other); }
		bool IsNearlyEqual(const Vector2& other, const float threshold) { return Vector2::IsNearlyEqual(*this, other, threshold); }

		//Operators

		Vector2 operator+(const Vector2& other) const { return Vector2::Add(*this, other); }
		Vector2 operator-(const Vector2& other) const { return Vector2::Sub(*this, other); }
		Vector2 operator*(const Vector2& other) const { return Vector2::Mul(*this, other); }
		Vector2 operator/(const Vector2& other) const { return Vector2::Div(*this, other); }
		bool operator==(const Vector2& other) const { return Vector2::IsEqual(*this, other); }
		bool operator!=(const Vector2& other) const { return !Vector2::IsEqual(*this, other); }

		Vector2 operator+(const float rhs) const { return Vector2::Add(*this, rhs); }
		Vector2 operator-(const float rhs) const { return Vector2::Sub(*this, rhs); }
		Vector2 operator*(const float rhs) const { return Vector2::Mul(*this, rhs); }
		Vector2 operator/(const float rhs) const { return Vector2::Div(*this, rhs); }

		friend Vector2 operator+(const float& lhs, const Vector2& rhs) { return Vector2::Add(rhs, lhs); }
		friend Vector2 operator-(const float& lhs, const Vector2& rhs) { return Vector2::Sub(rhs, lhs); }
		friend Vector2 operator*(const float& lhs, const Vector2& rhs) { return Vector2::Mul(rhs, lhs); }
		friend Vector2 operator/(const float& lhs, const Vector2& rhs) { return Vector2::Div(rhs, lhs); }
	};

}