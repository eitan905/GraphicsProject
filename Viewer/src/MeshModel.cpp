#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)

{
	localRotateBarValue_X = 0;
	localRotateBarValue_Y = 0;
	localRotateBarValue_Z = 0;
	localScaleBarValue = 0;


	objectTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	localRotationTransform_X =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	localRotationTransform_Y =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	localRotationTransform_Z =
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
	LocalRotationTransform_Z(localRotateBarValue_Z);
	LocalRotationTransform_Y(localRotateBarValue_Y);
	LocalRotationTransform_X(localRotateBarValue_X);
	glm::mat4x4 temp = localScaleTransform;
	temp[0][0] += localScaleBarValue;
	temp[1][1] += localScaleBarValue;
	temp[2][2] += localScaleBarValue;

	/*std::cout << localTranslateTransform[0][0] << "," << localTranslateTransform[0][1] << "," << localTranslateTransform[0][2] << "," << localTranslateTransform[0][3] << std::endl;
	std::cout << localTranslateTransform[1][0] << "," << localTranslateTransform[1][1] << "," << localTranslateTransform[1][2] << "," << localTranslateTransform[1][3] << std::endl;
	std::cout << localTranslateTransform[2][0] << "," << localTranslateTransform[2][1] << "," << localTranslateTransform[2][2] << "," << localTranslateTransform[2][3] << std::endl;*/
	//std::cout << localTranslateTransform[3][0] << "," << localTranslateTransform[3][1] << "," << localTranslateTransform[3][2] << "," << localTranslateTransform[3][3] << std::endl;


	objectTransform = localTranslateTransform * temp * localRotationTransform_X * 
		localRotationTransform_Y * localRotationTransform_Z;
}
//get the world transfrmation
void MeshModel::GETworld() {
	 worldTransform = worldTranslateTransform * worldScaleTransform * worldRotationTransform;
}


//set Local Rotation Transform by getting alfa parameter (in degrees)
void MeshModel::LocalRotationTransform_Z(const float alfa) {
	localRotateBarValue_Z = localRotateBarValue_Z + alfa;
	localRotationTransform_Z[0][0] = cos((localRotateBarValue_Z * 3.14) /  (2 * 180));
	localRotationTransform_Z[0][1] = sin((localRotateBarValue_Z * 3.14) / (2 * 180));
	localRotationTransform_Z[1][0] = -sin((localRotateBarValue_Z * 3.14) / (2 * 180));
	localRotationTransform_Z[1][1] = cos((localRotateBarValue_Z * 3.14) / (2 * 180));
}
void MeshModel::LocalRotationTransform_Y(const float alfa) {
	localRotateBarValue_Y = localRotateBarValue_Y + alfa;
	localRotationTransform_Y[0][0] = cos((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[0][2] = sin((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[2][0] = -sin((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[2][2] = cos((localRotateBarValue_Y * 3.14) / (2 * 180));
}
void MeshModel::LocalRotationTransform_X(const float alfa) {
	localRotateBarValue_X = localRotateBarValue_X + alfa;
	localRotationTransform_X[1][1] = cos((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[1][2] = sin((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[2][1] = -sin((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[2][2] = cos((localRotateBarValue_X * 3.14) / (2 * 180));
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
	/*std::cout << localTranslateTransform[0][0] << "," << localTranslateTransform[0][1] << "," << localTranslateTransform[0][2] << "," << localTranslateTransform[0][3] << std::endl;
	std::cout << localTranslateTransform[1][0] << "," << localTranslateTransform[1][1] << "," << localTranslateTransform[1][2] << "," << localTranslateTransform[1][3] << std::endl;
	std::cout << localTranslateTransform[2][0] << "," << localTranslateTransform[2][1] << "," << localTranslateTransform[2][2] << "," << localTranslateTransform[2][3] << std::endl;
	std::cout << localTranslateTransform[3][0] << "," << localTranslateTransform[3][1] << "," << localTranslateTransform[3][2] << "," << localTranslateTransform[3][3] << std::endl;*/
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

glm::mat4x4 MeshModel::GetTranslateTransform()
{
	return localTranslateTransform;
}

glm::mat4x4 MeshModel::GetScaleTransform()
{
	return localScaleTransform;
}

std::vector<glm::vec3> MeshModel::GetNormals()
{
	return normals_;
}

std::vector<Face> MeshModel::GetFaces()
{
	return faces_;
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
//
// set rotate bar value



