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
	void SetWorldTransform(glm::mat4x4 mat);
	static glm::mat4x4 worldTransform;

private:
	glm::mat4x4 objectTransform;
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
};
