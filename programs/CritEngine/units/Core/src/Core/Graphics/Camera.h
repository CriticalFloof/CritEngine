#pragma once

#include "../Math/Common.h"


namespace Engine
{
    class ENGINE_API BaseCamera
    {
    public:
        BaseCamera(float aspect_ratio = (16.f / 9.f), float near_plane = 0.01f, float far_plane = 1000.f,
                   Vector3 position = Vector3(0, 0, 0), Quaternion rotation = Quaternion());
        virtual ~BaseCamera() = default;
        BaseCamera(const BaseCamera&) = default;
        BaseCamera& operator=(const BaseCamera&) = default;

        void setPosition(const Vector3& new_position);
        void setRotation(const Quaternion& new_rotation);
        void setAspectRatio(float new_aspect_ratio);
        void setNearPlane(float new_near_plane);
        void setFarPlane(float new_far_plane);

        [[nodiscard]] Vector3 getPosition() const;
        [[nodiscard]] Quaternion getRotation() const;
        [[nodiscard]] float getAspectRatio() const;
        [[nodiscard]] float getNearPlane() const;
        [[nodiscard]] float getFarPlane() const;

        [[nodiscard]] Matrix4f getViewPerspectiveMatrix() const;
        [[nodiscard]] Matrix4f getPerspectiveMatrix() const;
        [[nodiscard]] Matrix4f getViewMatrix() const;

    protected:
        virtual void calculateViewMatrix() = 0;
        virtual void calculatePerspectiveMatrix() = 0;

        Matrix4f m_viewMatrix = Matrix4f::identity();
        Matrix4f m_perspectiveMatrix = Matrix4f::identity();
        Matrix4f m_viewPerspectiveMatrix = Matrix4f::identity();

        Quaternion m_rotation;
        Vector3 m_position;
        float m_aspectRatio;
        float m_nearPlane;
        float m_farPlane;
    };

    class ENGINE_API PerspectiveCamera : public BaseCamera
    {
    public:
        PerspectiveCamera(float vertical_fov = 60.f, float aspect_ratio = (16.f / 9.f), float near_plane = 0.01f,
                          float far_plane = 1000.f, Vector3 position = Vector3(0, 0, 0),
                          Quaternion rotation = Quaternion());

        void setVerticalFov(float new_vertical_fov);
        [[nodiscard]] float getVerticalFov() const;

    protected:
        void calculateViewMatrix() override;
        void calculatePerspectiveMatrix() override;

        float m_verticalFov;
    };

    class ENGINE_API OrthographicCamera : public BaseCamera
    {
    public:
        OrthographicCamera(float height = 720.f, float aspect_ratio = (16.f / 9.f), float near_plane = 0.01f,
                           float far_plane = 1000.f, Vector3 position = Vector3(0, 0, 0),
                           Quaternion rotation = Quaternion());

        void setHeight(float new_height);
        [[nodiscard]] float getHeight() const;

    protected:
        void calculateViewMatrix() override;
        void calculatePerspectiveMatrix() override;

        float m_height;
    };
}
