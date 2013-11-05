#include "engine.h"

const Matrix4f& Pipeline::GetWorldTransformation(bool bRotateAroundPivot, bool foo)
{
    Matrix4f mat4ScaleTransformation, mat4RotationTransformation, mat4TranslationTransformation;

    mat4ScaleTransformation.InitScaleTransform(m_vec3Scale.x, m_vec3Scale.y, m_vec3Scale.z);
    mat4RotationTransformation.InitRotateTransform(m_vec3RotationInfo.x, m_vec3RotationInfo.y, m_vec3RotationInfo.z);
    mat4TranslationTransformation.InitTranslationTransform(m_vec3WorldPosition.x, m_vec3WorldPosition.y, m_vec3WorldPosition.z);

    if (bRotateAroundPivot) 
    {
        m_mat4WorldTransformation = mat4RotationTransformation * mat4TranslationTransformation * mat4ScaleTransformation;
    }
    else
    {
        m_mat4WorldTransformation = mat4TranslationTransformation * mat4RotationTransformation * mat4ScaleTransformation;
    }

    return m_mat4WorldTransformation;
}

const Matrix4f& Pipeline::GetWorldPerspectiveTransformation(bool bRotateAroundPivot, bool foo)
{
    GetWorldTransformation(bRotateAroundPivot, foo);

    Matrix4f mat4CameraTranslationTransformation, mat4CameraRotationTransformation, mat4PerspectiveProjectionTransformation;

    mat4CameraTranslationTransformation.InitTranslationTransform(-m_Camera.vec3Position.x, -m_Camera.vec3Position.y, -m_Camera.vec3Position.z);
    mat4CameraRotationTransformation.InitCameraTransform(m_Camera.vec3Target, m_Camera.vec3Up);
    mat4PerspectiveProjectionTransformation.InitPersProjTransform(m_PerspectiveProjection.fFov, m_PerspectiveProjection.fWidth, m_PerspectiveProjection.fHeight, m_PerspectiveProjection.fZNear, m_PerspectiveProjection.fZFar);

    m_mat4WorldPerspectiveTransformation = mat4PerspectiveProjectionTransformation * mat4CameraTranslationTransformation * m_mat4WorldTransformation * mat4CameraRotationTransformation;

    return m_mat4WorldPerspectiveTransformation;
}

