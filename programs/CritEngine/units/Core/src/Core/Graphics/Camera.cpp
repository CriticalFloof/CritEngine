#include "Camera.h"

#include <numbers>

namespace Engine {
	BaseCamera::BaseCamera(
		const float aspect_ratio, 
		const float near_plane, 
		const float far_plane, 
		const Vector3 position, 
		const Quaternion rotation
	) : m_rotation(rotation), m_position(position), m_aspectRatio(aspect_ratio), m_nearPlane(near_plane), m_farPlane(far_plane)
	{
	}

	void BaseCamera::setPosition(const Vector3& new_position)
	{
		m_position = new_position;
		calculateViewMatrix();
	}

	void BaseCamera::setRotation(const Quaternion& new_rotation)
	{
		m_rotation = new_rotation;
		calculateViewMatrix();
	}

	void BaseCamera::setAspectRatio(float new_aspect_ratio)
	{
		m_aspectRatio = new_aspect_ratio;
		calculatePerspectiveMatrix();
	}

	void BaseCamera::setNearPlane(float new_near_plane)
	{
		m_nearPlane = new_near_plane;
		calculatePerspectiveMatrix();
	}

	void BaseCamera::setFarPlane(float new_far_plane)
	{
		m_farPlane = new_far_plane;
		calculatePerspectiveMatrix();
	}

	Vector3 BaseCamera::getPosition() const { return m_position; }
	Quaternion BaseCamera::getRotation() const { return m_rotation; }
	float BaseCamera::getAspectRatio() const { return m_aspectRatio; }
	float BaseCamera::getNearPlane() const { return m_nearPlane; }
	float BaseCamera::getFarPlane() const { return m_farPlane; }
	
	Matrix4f BaseCamera::getViewPerspectiveMatrix() const { return m_viewPerspectiveMatrix; }
	Matrix4f BaseCamera::getPerspectiveMatrix() const { return m_perspectiveMatrix; }
	Matrix4f BaseCamera::getViewMatrix() const {return m_viewMatrix; }


	// Perspective 
	PerspectiveCamera::PerspectiveCamera(
		const float vertical_fov,
		const float aspect_ratio,
		const float near_plane,
		const float far_plane,
		const Vector3 position,
		const Quaternion rotation
	) : BaseCamera(aspect_ratio, near_plane, far_plane, position, rotation), m_verticalFov(vertical_fov)
	{
		PerspectiveCamera::calculatePerspectiveMatrix();
		PerspectiveCamera::calculateViewMatrix();
	}

	float PerspectiveCamera::getVerticalFOV() const { return m_verticalFov; }
	
	void PerspectiveCamera::setVerticalFov(const float new_vertical_fov)
	{
		m_verticalFov = new_vertical_fov; 
		calculateViewMatrix();
	}

	void PerspectiveCamera::calculateViewMatrix()
	{
		const Quaternion q = m_rotation;
		const Vector3 p = m_position;
		
		m_viewMatrix = Matrix4f({
			1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
			2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
			2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
			// 4th row
			-p.x * (1 - 2 * q.y * q.y - 2 * q.z * q.z) - p.y * (2 * q.x * q.y - 2 * q.w * q.z) - p.z * (2 * q.x * q.z + 2 * q.w * q.y), 
			-p.x * (2 * q.x * q.y + 2 * q.w * q.z) - p.y * (1 - 2 * q.x * q.x - 2 * q.z * q.z) - p.z * (2 * q.y * q.z - 2 * q.w * q.x), 
			-p.x * (2 * q.x * q.z - 2 * q.w * q.y) - p.y * (2 * q.y * q.z + 2 * q.w * q.x) - p.z * (1 - 2 * q.x * q.x - 2 * q.y * q.y), 
			1
		});

		m_viewPerspectiveMatrix = m_perspectiveMatrix * m_viewMatrix;
	}

	void PerspectiveCamera::calculatePerspectiveMatrix()
	{
		const float far = m_farPlane;
		const float near = m_nearPlane;
		const float fov_rad = m_verticalFov / 180.f * std::numbers::pi_v<float>;
		const float tan_fov_y = tanf(fov_rad / 2.f);

		m_perspectiveMatrix = Matrix4f({
			1 / (m_aspectRatio * tan_fov_y), 0, 0, 0,
			0, 1 / tan_fov_y, 0, 0,
			0, 0, -(far + near) / (far + near), -1,
			0, 0, -(2 * far * near) / (far - near), 0
		});

		m_viewPerspectiveMatrix = m_perspectiveMatrix * m_viewMatrix;
	}

	// Orthographic 
	OrthographicCamera::OrthographicCamera(
		const float height,
		const float aspect_ratio,
		const float near_plane,
		const float far_plane,
		const Vector3 position,
		const Quaternion rotation
	) : BaseCamera(aspect_ratio, near_plane, far_plane, position, rotation), m_height(height)
	{
		OrthographicCamera::calculatePerspectiveMatrix();
		OrthographicCamera::calculateViewMatrix();
	}
	
	float OrthographicCamera::getHeight() const { return m_height; }
	
	void OrthographicCamera::setHeight(const float new_height)
	{
		m_height = new_height; 
		calculateViewMatrix();
	}

	void OrthographicCamera::calculateViewMatrix()
	{
		const Quaternion q = m_rotation;
		const Vector3 p = m_position;
		
		m_viewMatrix = Matrix4f({
			1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
			2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
			2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
			// 4th row
			-p.x * (1 - 2 * q.y * q.y - 2 * q.z * q.z) - p.y * (2 * q.x * q.y - 2 * q.w * q.z) - p.z * (2 * q.x * q.z + 2 * q.w * q.y), 
			-p.x * (2 * q.x * q.y + 2 * q.w * q.z) - p.y * (1 - 2 * q.x * q.x - 2 * q.z * q.z) - p.z * (2 * q.y * q.z - 2 * q.w * q.x), 
			-p.x * (2 * q.x * q.z - 2 * q.w * q.y) - p.y * (2 * q.y * q.z + 2 * q.w * q.x) - p.z * (1 - 2 * q.x * q.x - 2 * q.y * q.y), 
			1
		});

		m_viewPerspectiveMatrix = m_perspectiveMatrix * m_viewMatrix;
	}

	void OrthographicCamera::calculatePerspectiveMatrix()
	{
		const float far = m_farPlane;
		const float near = m_nearPlane;
		const float top = m_height / 2;
		const float bottom = -m_height / 2;
		const float left = m_height * m_aspectRatio / 2;
		const float right = -m_height * m_aspectRatio / 2;

		m_perspectiveMatrix = Engine::Matrix4f({
			-2 / (right - left), 0, 0, 0,
			0, 2 / (top - bottom), 0, 0,
			0, 0, 2 / (far - near), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1,
		});

		m_viewPerspectiveMatrix = m_perspectiveMatrix * m_viewMatrix;
	}
}
