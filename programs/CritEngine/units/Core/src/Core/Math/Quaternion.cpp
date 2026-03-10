#include "Quaternion.h"
#include <cmath>

namespace Engine
{
    Quaternion Quaternion::fromEulerAngles(Vector3 angles)
    {
        
        
        float roll = angles.x;
        float pitch = angles.y;
        float yaw = angles.z;

        float cosRoll = std::cosf(roll * 0.5f);
        float sinRoll = std::sinf(roll * 0.5f);
        float cosPitch = std::cosf(pitch * 0.5f);
        float sinPitch = std::sinf(pitch * 0.5f);
        float cosYaw = std::cosf(yaw * 0.5f);
        float sinYaw = std::sinf(yaw * 0.5f);

        return Quaternion(
            sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
            cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
            cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
            cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
        );
    }

    Quaternion Quaternion::fromAxisAngle(const Vector3 axis, float angle)
    {
        Vector3 normalizedAxis = axis.normalize();
        float sinHalfAngle = std::sinf(angle * 0.5f);
        float cosHalfAngle = std::cosf(angle * 0.5f);
        return Quaternion(cosHalfAngle, normalizedAxis.x * sinHalfAngle, normalizedAxis.y * sinHalfAngle,
                          normalizedAxis.z * sinHalfAngle);
    }

    Quaternion Quaternion::mul(const Quaternion& first, const Quaternion& second)
    {
        return Quaternion(
            first.w * second.x + first.x * second.w + first.y * second.z - first.z * second.y,
            first.w * second.y - first.x * second.z + first.y * second.w + first.z * second.x,
            first.w * second.z + first.x * second.y - first.y * second.x + first.z * second.w,
            first.w * second.w - first.x * second.x - first.y * second.y - first.z * second.z
        );
    }

    Quaternion Quaternion::log(const Quaternion& base)
    {
        float magnitude = std::sqrt(base.x * base.x + base.y * base.y + base.z * base.z + base.w * base.w);

        float theta = std::acos(base.w / magnitude);

        float vectorMagnitude = std::sqrt(base.x * base.x + base.y * base.y + base.z * base.z);
        float unit_x = base.x / vectorMagnitude;
        float unit_y = base.y / vectorMagnitude;
        float unit_z = base.z / vectorMagnitude;


        return Quaternion(unit_x * theta, unit_y * theta, unit_z * theta, std::log(magnitude));
    }

    Quaternion Quaternion::exp(const Quaternion& base)
    {
        float vectorMagnitude = std::sqrt(base.x * base.x + base.y * base.y + base.z * base.z);
        float unit_x = base.x / vectorMagnitude;
        float unit_y = base.y / vectorMagnitude;
        float unit_z = base.z / vectorMagnitude;

        float exp_w = std::exp(base.w);

        float sinMagnitude = std::sin(vectorMagnitude);

        return Quaternion(
            exp_w * sinMagnitude * unit_x,
            exp_w * sinMagnitude * unit_y,
            exp_w * sinMagnitude * unit_z,
            exp_w * std::cos(vectorMagnitude)
        );
    }

    Quaternion Quaternion::conjugate(const Quaternion& base)
    {
        return Quaternion(-base.x, -base.y, -base.z, base.w);
    }

    Quaternion Quaternion::normalize(const Quaternion& base)
    {
        float length = std::sqrtf(base.x * base.x + base.y * base.y + base.z * base.z + base.w * base.w);
        if (length > 0.0f)
        {
            return Quaternion(
                base.x / length,
                base.y / length,
                base.z / length,
                base.w / length
            );
        }
        return Quaternion();
    }

    Quaternion Quaternion::slerpShort(const Quaternion& start, const Quaternion& end, float alpha)
    {
        float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

        float flipped_mult = 1.f;
        if (dot < 0.0f)
        {
            flipped_mult = -1.f;
            dot = -dot;
        }

        if (dot > 0.9995f)
        {
            Quaternion result(
                start.x + alpha * (end.x * flipped_mult - start.x),
                start.y + alpha * (end.y * flipped_mult - start.y),
                start.z + alpha * (end.z * flipped_mult - start.z),
                start.w + alpha * (end.w * flipped_mult - start.w)
            );
            return result.normalize();
        }
        float theta_0 = std::acos(dot);
        float theta = theta_0 * alpha;
        float sin_theta = std::sin(theta_0);
        float sin_theta_t = std::sin(theta);

        float s0 = std::cos(theta) - dot * sin_theta_t / sin_theta;
        float s1 = sin_theta_t / sin_theta;

        return Quaternion(
            s0 * start.x + s1 * end.x * flipped_mult,
            s0 * start.y + s1 * end.y * flipped_mult,
            s0 * start.z + s1 * end.z * flipped_mult,
            s0 * start.w + s1 * end.w * flipped_mult
        );
    }

    Quaternion Quaternion::slerpLong(const Quaternion& start, const Quaternion& end, float alpha)
    {
        float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

        float flipped_mult = 1.f;
        if (dot > 0.0f)
        {
            flipped_mult = -1.f;
            dot = -dot;
        }

        if (dot < -0.9995f)
        {
            Quaternion result(
                start.x + alpha * (end.x * flipped_mult - start.x),
                start.y + alpha * (end.y * flipped_mult - start.y),
                start.z + alpha * (end.z * flipped_mult - start.z),
                start.w + alpha * (end.w * flipped_mult - start.w)
            );
            return result.normalize();
        }
        float theta_0 = std::acos(dot);
        float theta = theta_0 * alpha;
        float sin_theta = std::sin(theta_0);
        float sin_theta_t = std::sin(theta);

        float s0 = std::cos(theta) - dot * sin_theta_t / sin_theta;
        float s1 = sin_theta_t / sin_theta;

        return Quaternion(
            s0 * start.x + s1 * end.x * flipped_mult,
            s0 * start.y + s1 * end.y * flipped_mult,
            s0 * start.z + s1 * end.z * flipped_mult,
            s0 * start.w + s1 * end.w * flipped_mult
        );
    }

    // Implementation based off https://www.3dgep.com/understanding-quaternions/#squad
    // This function is not tested, once a renderer has been built, test this function for its correctness.
    Quaternion Quaternion::squadSpline(const std::vector<Quaternion>& spline, float alpha)
    {
        int start_index = static_cast<int>((spline.size() - 1) * alpha);
        if (start_index == spline.size()) start_index--;
        int end_index = start_index + 1;

        auto spline_helper = [](Quaternion qn1, Quaternion q0, Quaternion q1) -> Quaternion
        {
            Quaternion minus1 = log(qn1 * q0.conjugate());
            Quaternion plus1 = log(q1 * q0.conjugate());
            auto log_diff_scaled = Quaternion((minus1.x - plus1.x) / 4, (minus1.y - plus1.y) / 4,
                                              (minus1.z - plus1.z) / 4, (minus1.w - plus1.w) / 4);

            return log_diff_scaled.exp().mul(q0);
        };

        Quaternion s0;
        if (start_index > 0)
        {
            s0 = spline_helper(spline[start_index - 1], spline[start_index], spline[start_index + 1]);
        }
        else
        {
            s0 = spline[start_index + 1];
        }

        Quaternion s1;
        if (start_index + 1 <= spline.size())
        {
            s1 = spline_helper(spline[start_index], spline[start_index + 1], spline[start_index + 2]);
        }
        else
        {
            s1 = spline[start_index - 2];
        }


        return slerpShort(
            slerpShort(spline[start_index], spline[start_index + 1], alpha),
            slerpShort(s0, s1, alpha),
            2 * alpha * (1 - alpha)
        );
    }

    Vector3 Quaternion::rotateVector(const Quaternion& base, const Vector3& vector)
    {
        const Vector3 quat_vector(base.x, base.y, base.z);
        const Vector3 intermediate = 2.f * vector.cross(quat_vector);
        return vector + (base.w * intermediate) + intermediate.cross(quat_vector);
    }

    Vector3 Quaternion::toEulerAngles(const Quaternion& base)
    {
        float roll = atan2f(2.0f * (base.w * base.x + base.y * base.z),
                            1.0f - 2.0f * (base.x * base.x + base.y * base.y));
        float pitch = asinf(std::clamp(2.0f * (base.w * base.y - base.z * base.x), -1.0f, 1.0f));
        float yaw = atan2f(2.0f * (base.w * base.z + base.x * base.y),
                           1.0f - 2.0f * (base.y * base.y + base.z * base.z));

        return Vector3(roll, pitch, yaw);
    }
}
