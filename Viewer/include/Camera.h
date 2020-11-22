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
	glm::mat4x4 c;
	glm::mat4x4 cinv;
	glm::mat4x4 localTranslateTransform;
	glm::mat4x4 projection_transformation_;
	glm::mat4x4 view_transformation_;
	/*Camera(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	glm::vec3& getVerticeAtIndex(int index) {
		return vertices_[index];
	}
	std::vector<glm::vec3>& getVertices() {
		return vertices_;
	}
	int getVerticesSize() {
		return vertices_.size();
	}
	glm::mat4x4 GetTransform();




	void SetModelName(std::string name);
	void SetScaleBarValue(float value);
	float& GetScaleBarValue();
	void SetRotateBarValue(float value);

	void Camera::GETlocal();
	void Camera::GETworld();

	void Camera::LocalRotationTransform(const float alfa);
	void Camera::WorldRotationTransform(const float alfa);
	void Camera::LocalTranslateTransform(const float x, const float y, const float z);
	void Camera::WorldTranslateTransform(const float x, const float y, const float z);
	void Camera::LocalScaleTransform(const float x, const float y, const float z);
	void Camera::WorldScaleTransform(const float x, const float y, const float z);
	float localRotateBarValue = 0;
	float position = 0;


	glm::mat4x4 objectTransform;
	glm::mat4x4 localRotationTransform;
	glm::mat4x4 localTranslateTransform;
	glm::mat4x4 localScaleTransform;

	glm::mat4x4 worldTransform;
	static glm::mat4x4 worldRotationTransform;
	static glm::mat4x4 worldTranslateTransform;
	static glm::mat4x4 worldScaleTransform;


	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;

	float localScaleBarValue;


	float worldRotateBarValue;


private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
	*/
};
