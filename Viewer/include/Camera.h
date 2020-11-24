#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void Camera::Translate(const glm::vec4& v);
	const glm::mat4x4& Camera::GetProjectionTransformation() const;
	const glm::mat4x4& Camera::GetViewTransformation() const;
	glm::mat4x4 Camera::LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	glm::vec4 Camera::normalization(const glm::vec3& v);
	glm::vec4 Camera::crossproduct(const glm::vec4& v1, const glm::vec4& v2);
	void Camera::TranslateSpace(float x, float y, float z);
	void Camera::TranslateWorld(float x, float y, float z);
	void Camera::TranslatLocal(float x, float y, float z);
	glm::mat4x4 GetTransform();
	void SetDistance(double value);
	glm::mat4x4 GetPerspectiveNormalization(double left, double right, double top, double bottom, double near, double far);
	glm::mat4x4 GetOrthoNormalization(double left, double right, double top, double bottom, double near, double far);
	glm::vec3 GetViewPortTransformation(glm::vec3 vec, float width, float height);


	
	void Camera::RotateLocal(float x);
	void Camera::ScaleLocal(float x, float y, float z);

	void Camera::RotateWorld(float x, float y, float z);
	void Camera::ScaleWorld(float x, float y, float z);

	void Camera::RotateWorld(float x);
	int localRotateBarValue;

	glm::mat4x4 c;
	glm::mat4x4 cinv;
	glm::mat4x4 projection_transformation__;
	glm::mat4x4 view_transformation__;
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
	glm::mat4x4 objectTransform;
	glm::mat4x4 localRotationTransform;
	glm::mat4x4 localTranslateTransform;
	glm::mat4x4 localScaleTransform;
	double distance;

	glm::mat4x4 worldTransform;
	static glm::mat4x4 worldRotationTransform;
	static glm::mat4x4 worldTranslateTransform;
	static glm::mat4x4 worldScaleTransform;
};