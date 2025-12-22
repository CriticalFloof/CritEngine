#pragma once

#include "Vector3.h"
#include "../Core/Base.h"

namespace Engine {

	struct ENGINE_API Quaternion
	{
		float x, y, z, w;
		Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

		static Quaternion FromEulerAngles(Vector3 angles);
		static Quaternion FromAxisAngle(const Vector3 axis, float angle);

		// Static Math Operations
		static Quaternion Mul(const Quaternion& first, const Quaternion& second);
		static Quaternion Log(const Quaternion& base);
		static Quaternion Exp(const Quaternion& base);
		static Quaternion Conjugate(const Quaternion& base);
		static Quaternion Normalize(const Quaternion& base);
		static Quaternion SlerpShort(const Quaternion& start, const Quaternion& end, float alpha);
		static Quaternion SlerpLong(const Quaternion& start, const Quaternion& end, float alpha);
		static Quaternion SquadSpline(const std::vector<Quaternion>& spline, float alpha); // TODO: Move this function to a spline class once we have a need for one.
		static Vector3 RotateVector(const Quaternion& base, const Vector3& vector);
		static Vector3 ToEulerAngles(const Quaternion& base);

		// Object Math Operations
		Quaternion Mul(Quaternion& base) const { return Quaternion::Mul(*this, base); }
		Quaternion Log() const { return Quaternion::Log(*this); }
		Quaternion Exp() const { return Quaternion::Exp(*this); }
		Quaternion Conjugate() const { return Quaternion::Conjugate(*this); }
		Quaternion Normalize() const { return Quaternion::Normalize(*this); }
		Vector3 RotateVector(const Vector3& vector) const { return Quaternion::RotateVector(*this, vector); }
		Vector3 ToEulerAngles() const { return Quaternion::ToEulerAngles(*this); }

		// Operators

		Quaternion operator*(const Quaternion& rhs) const { return Quaternion::Mul(*this, rhs); }

	};

}