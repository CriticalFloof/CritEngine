#pragma once

#include "../Core/Base.h"

namespace Engine {

	struct ENGINE_API Vector4
	{
		float x, y, z, w;

		float& r = x;
		float& g = y;
		float& b = z;
		float& a = w;

		Vector4();
		Vector4(float scalar);
		Vector4(float x, float y, float z, float w);

		//Static Math Operations
		static Vector4 Add(const Vector4& first, const Vector4& second);
		static Vector4 Sub(const Vector4& first, const Vector4& second);
		static Vector4 Mul(const Vector4& first, const Vector4& second);
		static Vector4 Div(const Vector4& first, const Vector4& second);
		static float Dot(const Vector4& first, const Vector4& second);

		static Vector4 Add(const Vector4& base, const float scalar);
		static Vector4 Sub(const Vector4& base, const float scalar);
		static Vector4 Mul(const Vector4& base, const float scalar);
		static Vector4 Div(const Vector4& base, const float scalar);

		static float Length(const Vector4& base);
		static Vector4 Normalize(const Vector4& base);

		static float Angle(const Vector4& first, const Vector4& second);

		static bool IsEqual(const Vector4& first, const Vector4& second);
		static bool IsNearlyEqual(const Vector4& first, const Vector4& second, const float threshold);

		//Object Math Operations
		Vector4 Add(const Vector4& other) const { return Vector4::Add(*this, other); }
		Vector4 Sub(const Vector4& other) const { return Vector4::Sub(*this, other); }
		Vector4 Mul(const Vector4& other) const { return Vector4::Mul(*this, other); }
		Vector4 Div(const Vector4& other) const { return Vector4::Div(*this, other); }
		float Dot(const Vector4& other) const { return Vector4::Dot(*this, other); }

		Vector4 Add(const float scalar) const { return Vector4::Add(*this, scalar); }
		Vector4 Sub(const float scalar) const { return Vector4::Sub(*this, scalar); }
		Vector4 Mul(const float scalar) const { return Vector4::Mul(*this, scalar); }
		Vector4 Div(const float scalar) const { return Vector4::Div(*this, scalar); }

		float Length() { return Vector4::Length(*this); }
		Vector4 Normalize() { return Vector4::Normalize(*this); }

		float Angle(const Vector4& other) { return Vector4::Angle(*this, other); }

		bool IsEqual(const Vector4& other) { return Vector4::IsEqual(*this, other); }
		bool IsNearlyEqual(const Vector4& other, const float threshold) { return Vector4::IsNearlyEqual(*this, other, threshold); }
	};

}