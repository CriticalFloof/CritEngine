#include "Camera.h"

namespace Engine {

	BaseCamera::BaseCamera(
		float aspectRatio, 
		float nearPlane, 
		float farPlane, 
		Vector3 position, 
		Quaternion rotation
	) : rotation(rotation), position(position), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
	{
	}

	////////////////////////////////////////////////////////
	//  Perspective Camera  ////////////////////////////////
	////////////////////////////////////////////////////////

	PerspectiveCamera::PerspectiveCamera(
		float verticalFOV,
		float aspectRatio,
		float nearPlane,
		float farPlane,
		Vector3 position,
		Quaternion rotation
	) : BaseCamera(aspectRatio, nearPlane, farPlane, position, rotation), verticalFOV(verticalFOV)
	{
		this->CalculatePerspectiveMatrix();
		this->CalculateViewMatrix();
	}

	Matrix4f PerspectiveCamera::GetViewPerspectiveMatrix() { return this->viewPerspectiveMatrix; }
	Matrix4f PerspectiveCamera::GetPerspectiveMatrix() { return this->perspectiveMatrix; }
	Matrix4f PerspectiveCamera::GetViewMatrix() { return this->viewMatrix; }

	void PerspectiveCamera::CalculateViewMatrix()
	{
		Quaternion q = this->rotation;
		Vector3 p = this->position;

		// In the future the view matrix should be created in one pass to avoid an unnessesary matrix multiply.

		Matrix4f rotation = Matrix4f({
			1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
			2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
			2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
			0, 0, 0, 1
		});

		Matrix4f position = Matrix4f({
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-p.x, -p.y, -p.z, 1
		});

		this->viewMatrix = rotation * position;

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
	}

	void PerspectiveCamera::CalculatePerspectiveMatrix()
	{
		float far = this->farPlane;
		float near = this->nearPlane;
		float fovRad = this->verticalFOV / 180.f * 3.14159265f;
		float tanFovY = tanf(fovRad / 2.f);

		this->perspectiveMatrix = Matrix4f({
			1 / (this->aspectRatio * tanFovY), 0, 0, 0,
			0, 1 / tanFovY, 0, 0,
			0, 0, -(far + near) / (far + near), -1,
			0, 0, -(2 * far * near) / (far - near), 0
		});

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
	}

	////////////////////////////////////////////////////////
	//  Orthographic Camera  ///////////////////////////////
	////////////////////////////////////////////////////////

	OrthographicCamera::OrthographicCamera(
		float height,
		float aspectRatio,
		float nearPlane,
		float farPlane,
		Vector3 position,
		Quaternion rotation
	)
		: BaseCamera(aspectRatio, nearPlane, farPlane, position, rotation), height(height)
	{
		this->CalculatePerspectiveMatrix();
		this->CalculateViewMatrix();
	}

	Matrix4f OrthographicCamera::GetViewPerspectiveMatrix() { return this->viewPerspectiveMatrix; }
	Matrix4f OrthographicCamera::GetPerspectiveMatrix() { return this->perspectiveMatrix; }
	Matrix4f OrthographicCamera::GetViewMatrix() { return this->viewMatrix; }

	void OrthographicCamera::CalculateViewMatrix()
	{
		Quaternion q = this->rotation;
		Vector3 p = this->position;

		// In the future the view matrix should be created in place to save on some cycles for every recalculation.

		Matrix4f rotation = Matrix4f({
			1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
			2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
			2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
			0, 0, 0, 1
		});

		Matrix4f position = Matrix4f({
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-p.x, -p.y, -p.z, 1
		});

		this->viewMatrix = rotation * position;

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;
	}

	void OrthographicCamera::CalculatePerspectiveMatrix()
	{

		float far = this->farPlane;
		float near = this->nearPlane;
		float top = this->height / 2;
		float bottom = -this->height / 2;
		float left = this->height * this->aspectRatio / 2;
		float right = -this->height * this->aspectRatio / 2;

		this->perspectiveMatrix = Engine::Matrix4f({
			-2 / (right - left), 0, 0, 0,
			0, 2 / (top - bottom), 0, 0,
			0, 0, 2 / (far - near), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1,
		});

		this->viewPerspectiveMatrix = this->perspectiveMatrix * this->viewMatrix;

	}
}