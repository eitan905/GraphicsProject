#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)
	
{
	rotateBarValue = 0;
	scaleBarValue = 1;
	objectTransform = glm::mat4x4(
		1 , 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
	rotationTransform =
	glm::mat4x4(
		cos((rotateBarValue * 3.14) / 180), sin((rotateBarValue * 3.14) / 180), 0, 0,
		-sin((rotateBarValue * 3.14) / 180), cos((rotateBarValue * 3.14) / 180), 0, 0,
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


void MeshModel::setRotationTransfrom(glm::mat4x4 mat) {
	rotationTransform = rotationTransform * mat;
}
glm::mat4x4 MeshModel::GetTransform()
{

	glm::mat4x4 temp = objectTransform * rotationTransform;
	temp[0][0] += scaleBarValue;
	temp[1][1] += scaleBarValue;
	temp[2][2] += scaleBarValue;
	return worldTransform * temp;
}

void MeshModel::SetLocalTransform(glm::mat4x4 mat)
{
	objectTransform = mat * objectTransform;
}

void MeshModel::SetLocalRotationTransform(glm::mat4x4 mat)
{
	objectTransform = objectTransform * mat;
}

void MeshModel::SetWorldTransform(glm::mat4x4 mat)
{
	worldTransform = mat * worldTransform;
}

void MeshModel::SetModelName(std::string name)
{
	model_name_ = name;
}

void MeshModel::SetScaleBarValue(float value)
{
	scaleBarValue = value;
}

float& MeshModel::GetScaleBarValue()
{
	return scaleBarValue;
}

void MeshModel::SetRotateBarValue(float value)
{
	rotateBarValue = value;
}


