#include "Quaternion.h"
#include <cmath>

namespace Engine {

	Quaternion Quaternion::FromEulerAngles(Vector3 angles)
	{
		float roll = angles.x;
		float pitch = angles.y;
		float yaw = angles.z;

		float cosRoll = cosf(roll * 0.5f);
		float sinRoll = sinf(roll * 0.5f);
		float cosPitch = cosf(pitch * 0.5f);
		float sinPitch = sinf(pitch * 0.5f);
		float cosYaw = cosf(yaw * 0.5f);
		float sinYaw = sinf(yaw * 0.5f);

		return Quaternion(
			sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
			cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
			cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
			cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
		);
	}

	Quaternion Quaternion::FromAxisAngle(const Vector3 axis, float angle)
	{
		Vector3 normalizedAxis = axis.Normalize();
		float sinHalfAngle = sinf(angle * 0.5f);
		float cosHalfAngle = cosf(angle * 0.5f);
		return Quaternion(cosHalfAngle, normalizedAxis.x * sinHalfAngle, normalizedAxis.y * sinHalfAngle, normalizedAxis.z * sinHalfAngle);
	}

	Quaternion Quaternion::Mul(const Quaternion& first, const Quaternion& second)
	{
		return Quaternion(
			first.w * second.x + first.x * second.w + first.y * second.z - first.z * second.y,
			first.w * second.y - first.x * second.z + first.y * second.w + first.z * second.x,
			first.w * second.z + first.x * second.y - first.y * second.x + first.z * second.w,
			first.w * second.w - first.x * second.x - first.y * second.y - first.z * second.z
		);
	}

	Quaternion Quaternion::Log(const Quaternion& base)
	{
		float magnitude = sqrt(base.x * base.x + base.y * base.y + base.z * base.z + base.w * base.w);

		float theta = acos(base.w / magnitude);

		float vectorMagnitude = sqrt(base.x * base.x + base.y * base.y + base.z * base.z);
		float unit_x = base.x / vectorMagnitude;
		float unit_y = base.y / vectorMagnitude;
		float unit_z = base.z / vectorMagnitude;


		return Quaternion(unit_x * theta, unit_y * theta, unit_z * theta, log(magnitude));
	}

	Quaternion Quaternion::Exp(const Quaternion& base)
	{
		float vectorMagnitude = sqrt(base.x * base.x + base.y * base.y + base.z * base.z);
		float unit_x = base.x / vectorMagnitude;
		float unit_y = base.y / vectorMagnitude;
		float unit_z = base.z / vectorMagnitude;

		float exp_w = exp(base.w);

		float sinMagnitude = sin(vectorMagnitude);

		return Quaternion(
			exp_w * sinMagnitude * unit_x, 
			exp_w * sinMagnitude * unit_y, 
			exp_w * sinMagnitude * unit_z, 
			exp_w * cos(vectorMagnitude)
		);
	}

	Quaternion Quaternion::Conjugate(const Quaternion& base)
	{
		return Quaternion(-base.x, -base.y, -base.z, base.w);
	}

	Quaternion Quaternion::Normalize(const Quaternion& base)
	{
		float length = sqrtf(base.x * base.x + base.y * base.y + base.z * base.z + base.w * base.w);
		if (length > 0.0f)
		{
			return Quaternion(
				base.x / length,
				base.y / length,
				base.z / length,
				base.w / length
			);
		}
		else
		{
			return Quaternion();
		}
	}

	Quaternion Quaternion::SlerpShort(const Quaternion& start, const Quaternion& end, float alpha)
	{
		float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

		int flippedMult = 1;
		if (dot < 0.0f)
		{

			flippedMult = -1;
			dot = -dot; 
		}

		if (dot > 0.9995f)
		{
			Quaternion result(
				start.x + alpha * (end.x * flippedMult - start.x),
				start.y + alpha * (end.y * flippedMult - start.y),
				start.z + alpha * (end.z * flippedMult - start.z),
				start.w + alpha * (end.w * flippedMult - start.w)
			);
			result.Normalize();
			return result;
		}
		else
		{
			float theta_0 = std::acos(dot);
			float theta = theta_0 * alpha;
			float sin_theta = std::sin(theta_0);
			float sin_theta_t = std::sin(theta);
			
			float s0 = std::cos(theta) - dot * sin_theta_t / sin_theta;
			float s1 = sin_theta_t / sin_theta;

			return Quaternion(
				s0 * start.x + s1 * end.x * flippedMult,
				s0 * start.y + s1 * end.y * flippedMult,
				s0 * start.z + s1 * end.z * flippedMult,
				s0 * start.w + s1 * end.w * flippedMult
			);
		}
	}

	Quaternion Quaternion::SlerpLong(const Quaternion& start, const Quaternion& end, float alpha)
	{
		float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

		int flippedMult = 1;
		if (dot > 0.0f)
		{

			flippedMult = -1;
			dot = -dot;
		}

		if (dot < -0.9995f)
		{
			Quaternion result(
				start.x + alpha * (end.x * flippedMult - start.x),
				start.y + alpha * (end.y * flippedMult - start.y),
				start.z + alpha * (end.z * flippedMult - start.z),
				start.w + alpha * (end.w * flippedMult - start.w)
			);
			result.Normalize();
			return result;
		}
		else
		{
			float theta_0 = std::acos(dot);
			float theta = theta_0 * alpha;
			float sin_theta = std::sin(theta_0);
			float sin_theta_t = std::sin(theta);

			float s0 = std::cos(theta) - dot * sin_theta_t / sin_theta;
			float s1 = sin_theta_t / sin_theta;

			return Quaternion(
				s0 * start.x + s1 * end.x * flippedMult,
				s0 * start.y + s1 * end.y * flippedMult,
				s0 * start.z + s1 * end.z * flippedMult,
				s0 * start.w + s1 * end.w * flippedMult
			);
		}
	}

	// Implementation based off https://www.3dgep.com/understanding-quaternions/#squad
	// This function is not tested, once a renderer has been built, test this function for its correctness.
	Quaternion Quaternion::SquadSpline(const std::vector<Quaternion>& spline, float alpha)
	{
		int startIndex = static_cast<int>((spline.size() - 1) * alpha);
		if (startIndex == spline.size()) startIndex--;
		int endIndex = startIndex + 1;

		auto splineHelper = [](Quaternion qn1, Quaternion q0, Quaternion q1)
		{
			Quaternion minus1 = Quaternion::Log(qn1 * q0.Conjugate());
			Quaternion plus1 = Quaternion::Log(q1 * q0.Conjugate());
			Quaternion log_diff_scaled = Quaternion((minus1.x - plus1.x) / 4, (minus1.y - plus1.y) / 4, (minus1.z - plus1.z) / 4, (minus1.w - plus1.w) / 4);

			return log_diff_scaled.Exp().Mul(q0);
		};

		Quaternion s0;
		if (startIndex > 0)
		{
			s0 = splineHelper(spline[startIndex - 1], spline[startIndex], spline[startIndex + 1]);
		}
		else
		{
			s0 = spline[startIndex + 1];
		}

		Quaternion s1;
		if (startIndex + 1 <= spline.size())
		{
			s1 = splineHelper(spline[startIndex], spline[startIndex + 1], spline[startIndex + 2]);
		}
		else
		{
			s1 = spline[startIndex - 2];
		}


		return Quaternion::SlerpShort(
			Quaternion::SlerpShort(spline[startIndex], spline[startIndex + 1], alpha),
			Quaternion::SlerpShort(s0, s1, alpha),
			2 * alpha * (1 - alpha)
		);
	}

	Vector3 Quaternion::RotateVector(const Quaternion& base, const Vector3& vector)
	{
		const Vector3 quatVector(base.x, base.y, base.z);
		const Vector3 intermediate = 2.f * vector.Cross(quatVector);
		return vector + (base.w * intermediate) + intermediate.Cross(quatVector);
	}

	Vector3 Quaternion::ToEulerAngles(const Quaternion& base)
	{
		float roll = atan2f(2.0f * (base.w * base.x + base.y * base.z), 1.0f - 2.0f * (base.x * base.x + base.y * base.y));
		float pitch = asinf(std::clamp(2.0f * (base.w * base.y - base.z * base.x), -1.0f, 1.0f));
		float yaw = atan2f(2.0f * (base.w * base.z + base.x * base.y), 1.0f - 2.0f * (base.y * base.y + base.z * base.z));

		return Vector3(roll, pitch, yaw);
	}
}