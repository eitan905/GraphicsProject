#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals)
{
	objectTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
	
}
glm::mat4x4 MeshModel::worldTransform = worldTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}


const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

glm::mat4x4 MeshModel::GetTransform()
{
	return worldTransform * objectTransform;
}

void MeshModel::SetLocalTransform(glm::mat4x4 mat)
{
	objectTransform = mat * objectTransform;
}

void MeshModel::SetWorldTransform(glm::mat4x4 mat)
{
	worldTransform = mat * worldTransform;
}
