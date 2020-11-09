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
	void SetLocalTransform(glm::mat4x4 mat);
	void SetLocalRotationTransform(glm::mat4x4 mat);
	void SetWorldTransform(glm::mat4x4 mat);
	void SetModelName(std::string name);
	void SetScaleBarValue(float value);
	float& GetScaleBarValue();
	void SetRotateBarValue(float value);
	void setRotationTransfrom(glm::mat4x4 mat);

	void MeshModel::GETlocal();
	void MeshModel::GETworld();
	void MeshModel::setRotationTransfromLOCAL(const float&alfa);
	glm::mat4x4 MeshModel::GETMAT();
	void MeshModel::setRotationTransfromWORLD(const float&alfa);
	void MeshModel::setTranslateTransfromLOCAL(const float& x, const float& y, const float& z);
	void MeshModel::setTranslateTransfromWORLD(const float& x, const float& y, const float& z);
	void MeshModel::setScaleTransfromLOCAL(const float& x, const float& y, const float& z);
	void MeshModel::setScaleTransfromWORLD(const float& x, const float& y, const float& z);


private:
	glm::mat4x4 objectTransform;
	glm::mat4x4 rotationTransform;
	glm::mat4x4 translateTransform;
	glm::mat4x4 ScaleTransform;

	glm::mat4x4 worldTransform;
	glm::mat4x4 WrotationTransform;
	glm::mat4x4 WtranslateTransform;
	glm::mat4x4 WScaleTransform;


	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;

	float scaleBarValue;
	float rotateBarValue;
	float translateBarValueY;
	float translateBarValueX;
	float translateBarValueZ;
	float scaleValueX;
	float scaleValueY;
	float scaleValueZ;

	float WrotateBarValue;
	float WtranslateBarValueY;
	float WtranslateBarValueX;
	float WtranslateBarValueZ;
	float WscaleValueX;
	float WscaleValueY;
	float WscaleValueZ;
};
