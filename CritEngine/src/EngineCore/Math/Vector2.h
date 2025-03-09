#pragma once

#include "../Core/Base.h"

namespace Engine {

	struct ENGINE_API Vector2
	{
		float x, y;

		float& r = x;
		float& g = y;

		Vector2();
		Vector2(float scalar);
		Vector2(float x, float y);

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
	};

}