#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
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

	void MeshModel::GETlocal();
	void MeshModel::GETworld();

	void MeshModel::LocalRotationTransform(const float alfa);
	void MeshModel::WorldRotationTransform(const float alfa);
	void MeshModel::LocalTranslateTransform(const float x, const float y, const float z);
	void MeshModel::WorldTranslateTransform(const float x, const float y, const float z);
	void MeshModel::LocalScaleTransform(const float x, const float y, const float z);
	void MeshModel::WorldScaleTransform(const float x, const float y, const float z);
	

private:
	glm::mat4x4 objectTransform;
	glm::mat4x4 localRotationTransform;
	glm::mat4x4 localTranslateTransform;
	glm::mat4x4 localScaleTransform;

	glm::mat4x4 worldTransform;
	glm::mat4x4 worldRotationTransform;
	glm::mat4x4 worldTranslateTransform;
	glm::mat4x4 worldScaleTransform;


	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;

	float localScaleBarValue;
	float localRotateBarValue;
	

	float worldRotateBarValue;

};
