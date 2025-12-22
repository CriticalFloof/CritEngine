#pragma once

#include "../Math/Common.h"


namespace Engine {

	class ENGINE_API BaseCamera
	{
	public:
		BaseCamera(float aspectRatio = (16 / 9), float nearPlane = 0.01f, float farPlane = 1000.f, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());
		virtual ~BaseCamera() {};

		virtual Matrix4f GetViewPerspectiveMatrix() = 0;
		virtual Matrix4f GetPerspectiveMatrix() = 0;
		virtual Matrix4f GetViewMatrix() = 0;

		virtual void CalculateViewMatrix() = 0;
		virtual void CalculatePerspectiveMatrix() = 0;

		virtual void SetPosition(Vector3 newPosition) { this->position = newPosition; this->CalculateViewMatrix(); }
		virtual Vector3 GetPosition() { return this->position; }
		virtual void SetRotation(Quaternion newRotation) { this->rotation = newRotation; this->CalculateViewMatrix(); }
		virtual Quaternion GetRotation() { return this->rotation; }
		virtual void SetAspectRatio(float newAspectRatio) { this->aspectRatio = newAspectRatio; this->CalculatePerspectiveMatrix(); }
		virtual float GetAspectRatio() { return this->aspectRatio; }
		virtual void SetNearPlane(float newNearPlane) { this->nearPlane = newNearPlane; this->CalculatePerspectiveMatrix(); }
		virtual float GetNearPlane() { return this->nearPlane; }
		virtual void SetFarPlane(float newFarPlane) { this->farPlane = newFarPlane; this->CalculatePerspectiveMatrix(); }
		virtual float GetFarPlane() { return this->farPlane; }

	protected:
		Matrix4f viewMatrix = Matrix4f::Identity();
		Matrix4f perspectiveMatrix = Matrix4f::Identity();
		Matrix4f viewPerspectiveMatrix = Matrix4f::Identity();

		Quaternion rotation;
		Vector3 position;
		float aspectRatio;
		float nearPlane;
		float farPlane;
	};

	class ENGINE_API PerspectiveCamera : public BaseCamera
	{
	public:
		PerspectiveCamera(float verticalFOV = 60.f, float aspectRatio = (16/9), float nearPlane = 0.01f, float farPlane = 1000.f, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());
		virtual ~PerspectiveCamera() {};

		virtual Matrix4f GetViewPerspectiveMatrix() override;
		virtual Matrix4f GetPerspectiveMatrix() override;
		virtual Matrix4f GetViewMatrix() override;

		virtual void CalculateViewMatrix() override;
		virtual void CalculatePerspectiveMatrix() override;

		virtual void SetVertialFOV(float newVerticalFOV) { this->verticalFOV = newVerticalFOV; this->CalculateViewMatrix(); }
		virtual float GetVertialFOV() { return this->verticalFOV; }

	private:
		float verticalFOV;
	};		  

	class ENGINE_API OrthographicCamera : public BaseCamera
	{
	public:
		OrthographicCamera(float height = 720.f, float aspectRatio = (16/9), float nearPlane = 0.01f, float farPlane = 1000.f, Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());
		virtual ~OrthographicCamera() {};

		virtual Matrix4f GetViewPerspectiveMatrix() override;
		virtual Matrix4f GetPerspectiveMatrix() override;
		virtual Matrix4f GetViewMatrix() override;

		virtual void CalculateViewMatrix() override;
		virtual void CalculatePerspectiveMatrix() override;

		virtual void SetHeight(float newHeight) { this->height = newHeight; this->CalculateViewMatrix(); }
		virtual float GetHeight() { return this->height; }

	private:
		float height;
	};

}