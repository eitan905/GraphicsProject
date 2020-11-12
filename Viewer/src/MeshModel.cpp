#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)

{
	localRotateBarValue = 0;
	localScaleBarValue = 1;

	objectTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	localRotationTransform =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	localTranslateTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	localScaleTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);


	 worldTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	 worldRotateBarValue = 0;
	
	 
	 
}


glm::mat4x4 MeshModel::worldRotationTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);
glm::mat4x4 MeshModel::worldTranslateTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);
glm::mat4x4 MeshModel::worldScaleTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);


MeshModel::~MeshModel()
{
}
//get faces of the current model
const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}
//get faces count
int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

//get model name
const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

//get the local transformation 
void MeshModel::GETlocal() {
	glm::mat4x4 temp = localScaleTransform;
	temp[0][0] += localScaleBarValue;
	temp[1][1] += localScaleBarValue;
	temp[2][2] += localScaleBarValue;
	objectTransform = localTranslateTransform * temp * localRotationTransform;
}
//get the world transfrmation
void MeshModel::GETworld() {
	 worldTransform = worldTranslateTransform * worldScaleTransform * worldRotationTransform;
}


//set Local Rotation Transform by getting alfa parameter (in degrees)
void MeshModel::LocalRotationTransform(const float alfa) {
	localRotateBarValue = localRotateBarValue+ alfa;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);
}
//set World Rotation Transform by getting alfa parameter (in degrees)
void MeshModel::WorldRotationTransform(const float alfa) {
	worldRotateBarValue += alfa;
	worldRotationTransform[0][0] = cos((worldRotateBarValue * 3.14) / 180);
	worldRotationTransform[0][1] = sin((worldRotateBarValue * 3.14) / 180);
	worldRotationTransform[1][0] = -sin((worldRotateBarValue * 3.14) / 180);
	worldRotationTransform[1][1] = cos((worldRotateBarValue * 3.14) / 180);
}
//set Local Translate Transform by getting x,y,z parameter (parameters which determine the change)
void MeshModel::LocalTranslateTransform(const float x, const float y, const float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] += y;
	localTranslateTransform[3][2] += z;

}
//set World Translate Transform by getting x,y,z parameter (parameters which determine the change)
void MeshModel::WorldTranslateTransform(const float x, const float y, const float z) {
	worldTranslateTransform[3][0] += x;
	worldTranslateTransform[3][1] += y;
	worldTranslateTransform[3][2] += z;

}
//set Local Scale Transform by getting x,y,z parameter (parameters which determine the change)
void MeshModel::LocalScaleTransform(const float x, const float y, const float z)
{
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
}
//set World Scale Transform by getting x,y,z parameter (parameters which determine the change)
void MeshModel::WorldScaleTransform(const float x, const float y, const float z) {
	worldScaleTransform[0][0] *= x;
	worldScaleTransform[1][1] *= y;
	worldScaleTransform[2][2] *= z;

}


//Accepting the transformation by multiplying the global transformation by the local one
glm::mat4x4 MeshModel::GetTransform()
{
	GETlocal();
	GETworld();
	
	return worldTransform * objectTransform;
}

//set modal name
void MeshModel::SetModelName(std::string name)
{
	model_name_ = name;
}
// set scale bar value
void MeshModel::SetScaleBarValue(float value)
{
	localScaleBarValue = value;
}

// get scale bar value
float& MeshModel::GetScaleBarValue()
{
	return localScaleBarValue;
}

// set rotate bar value
void MeshModel::SetRotateBarValue(float value)
{
	localRotateBarValue = value;
	
}


