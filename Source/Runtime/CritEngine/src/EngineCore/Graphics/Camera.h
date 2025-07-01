#pragma once

#include "../Math/Common.h"


namespace Engine {

	class ENGINE_API PerspectiveCamera
	{
	public:

		PerspectiveCamera(float verticalFOV = 60.f, float aspectRatio = (16/9), float nearPlane = 0.01f, float farPlane = 1000.f, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());

		Matrix4f GetViewPerspectiveMatrix();
		Matrix4f GetPerspectiveMatrix();
		Matrix4f GetViewMatrix();

		void CalculateViewMatrix();
		void CalculatePerspectiveMatrix();

		void SetVerticalFOV(float verticalFOV);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		void SetPosition(Vector3 position);
		void SetRotation(Quaternion Rotation);
		Vector3 GetPosition();
		Quaternion GetRotation();

	private:
		Matrix4f viewMatrix = Matrix4f::Identity();
		Matrix4f perspectiveMatrix = Matrix4f::Identity();
		Matrix4f viewPerspectiveMatrix = Matrix4f::Identity();

		Quaternion rotation;
		Vector3 position;
		float verticalFOV;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	};		  

	class ENGINE_API OrthographicCamera
	{
	public:

		OrthographicCamera(float height, float aspectRatio, float nearPlane, float farPlane, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());

		Matrix4f GetViewPerspectiveMatrix();
		Matrix4f GetPerspectiveMatrix();
		Matrix4f GetViewMatrix();

		void CalculateViewMatrix();
		void CalculatePerspectiveMatrix();

		void SetHeight(float height);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		void SetPosition(Vector3 position);
		void SetRotation(Quaternion Rotation);
		Vector3 GetPosition();
		Quaternion GetRotation();

	private:
		Matrix4f viewMatrix = Matrix4f::Identity();
		Matrix4f perspectiveMatrix = Matrix4f::Identity();
		Matrix4f viewPerspectiveMatrix = Matrix4f::Identity();

		Quaternion rotation;
		Vector3 position;
		float height;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	};

}