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
	worldRotateBarValue_X = 0;
	worldRotateBarValue_Y = 0;
	worldRotateBarValue_Z = 0;
	localScaleBarValue = 0;
	position = 0;
		objPosition = glm::vec3(0, 0, 0);

	localTranslateBarValue_Y =
		localTranslateBarValue_X =
		localTranslateBarValue_Z = 0;
	worldTranslateBarValue_Y =
		worldTranslateBarValue_X =
		worldTranslateBarValue_Z = 0;
	localScaleBarValue =
		worldScaleBarValue = 1;
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
	worldRotationTransform_X = worldRotationTransform_Y = worldRotationTransform_Z = 
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

	 worldRotationTransform = glm::mat4x4(
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	 );
	 //s
	 worldTranslateTransform = glm::mat4x4(
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	 );
	 worldScaleTransform = glm::mat4x4(
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	 );

	 
	 
}





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
	LocalRotationTransform_Z();
	LocalRotationTransform_Y();
	LocalRotationTransform_X();
	glm::mat4x4 temp = localScaleTransform;
	temp[0][0] += localScaleBarValue;
	temp[1][1] += localScaleBarValue;
	temp[2][2] += localScaleBarValue;
	glm::mat4x4 temp2 = localTranslateTransform;
	temp2[3][0] += localTranslateBarValue_X;
	temp2[3][1] += localTranslateBarValue_Y;
	temp2[3][2] += localTranslateBarValue_Z;
	


	objectTransform = temp2 * temp * localRotationTransform_X * 
		localRotationTransform_Y * localRotationTransform_Z;
}
//get the world transfrmation
void MeshModel::GETworld() {
	WorldRotationTransform_Z();
	WorldRotationTransform_Y();
	WorldRotationTransform_X();
	glm::mat4x4 temp = worldScaleTransform;
	temp[0][0] += worldScaleBarValue;
	temp[1][1] += worldScaleBarValue;
	temp[2][2] += worldScaleBarValue;
	glm::mat4x4 temp2 = worldTranslateTransform;
	temp2[3][0] += worldTranslateBarValue_X;
	temp2[3][1] += worldTranslateBarValue_Y;
	temp2[3][2] += worldTranslateBarValue_Z;
	worldRotationTransform = worldRotationTransform_X * worldRotationTransform_Y * worldRotationTransform_Z;
	worldTransform = temp2 * temp * worldRotationTransform;
}


//set Local Rotation Transform by getting alfa parameter (in degrees)
void MeshModel::LocalRotationTransform_Z() {
	localRotationTransform_Z[0][0] = cos((localRotateBarValue_Z * 3.14) /  (2 * 180));
	localRotationTransform_Z[0][1] = sin((localRotateBarValue_Z * 3.14) / (2 * 180));
	localRotationTransform_Z[1][0] = -sin((localRotateBarValue_Z * 3.14) / (2 * 180));
	localRotationTransform_Z[1][1] = cos((localRotateBarValue_Z * 3.14) / (2 * 180));
}
void MeshModel::LocalRotationTransform_Y() {
	localRotationTransform_Y[0][0] = cos((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[0][2] = sin((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[2][0] = -sin((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[2][2] = cos((localRotateBarValue_Y * 3.14) / (2 * 180));
}
void MeshModel::LocalRotationTransform_X() {
	localRotationTransform_X[1][1] = cos((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[1][2] = sin((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[2][1] = -sin((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[2][2] = cos((localRotateBarValue_X * 3.14) / (2 * 180));
}

void MeshModel::WorldRotationTransform_Z() {
	worldRotationTransform_Z[0][0] = cos((worldRotateBarValue_Z * 3.14) / (2 * 180));
	worldRotationTransform_Z[0][1] = sin((worldRotateBarValue_Z * 3.14) / (2 * 180));
	worldRotationTransform_Z[1][0] = -sin((worldRotateBarValue_Z * 3.14) / (2 * 180));
	worldRotationTransform_Z[1][1] = cos((worldRotateBarValue_Z * 3.14) / (2 * 180));
}

void MeshModel::WorldRotationTransform_Y() {
	worldRotationTransform_Y[0][0] = cos((worldRotateBarValue_Y * 3.14) / (2 * 180));
	worldRotationTransform_Y[0][2] = sin((worldRotateBarValue_Y * 3.14) / (2 * 180));
	worldRotationTransform_Y[2][0] = -sin((worldRotateBarValue_Y * 3.14) / (2 * 180));
	worldRotationTransform_Y[2][2] = cos((worldRotateBarValue_Y * 3.14) / (2 * 180));
}

void MeshModel::WorldRotationTransform_X() {
	worldRotationTransform_X[1][1] = cos((worldRotateBarValue_X * 3.14) / (2 * 180));
	worldRotationTransform_X[1][2] = sin((worldRotateBarValue_X * 3.14) / (2 * 180));
	worldRotationTransform_X[2][1] = -sin((worldRotateBarValue_X * 3.14) / (2 * 180));
	worldRotationTransform_X[2][2] = cos((worldRotateBarValue_X * 3.14) / (2 * 180));
}

//set Local Translate Transform by getting x,y,z parameter (parameters which determine the change)
void MeshModel::LocalTranslateTransform(const float x, const float y, const float z) {
	localTranslateBarValue_X += x;
	localTranslateBarValue_Y += y;
	localTranslateBarValue_Z += z;

	
}
//set World Translate Transform by getting x,y,z parameter (parameters which determine the change)

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

void MeshModel::Reset()
{
	localRotateBarValue_X = 0;
	localRotateBarValue_Y = 0;
	localRotateBarValue_Z = 0;
	worldRotateBarValue_X = 0;
	worldRotateBarValue_Y = 0;
	worldRotateBarValue_Z = 0;
	localScaleBarValue = 0;

	localTranslateBarValue_Y =
		localTranslateBarValue_X =
		localTranslateBarValue_Z = 0;
	worldTranslateBarValue_Y =
		worldTranslateBarValue_X =
		worldTranslateBarValue_Z = 0;
	localScaleBarValue =
		worldScaleBarValue = 1;
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
	worldRotationTransform_X = worldRotationTransform_Y = worldRotationTransform_Z =
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

	worldRotationTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	//s
	worldTranslateTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	worldScaleTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);


}

glm::vec3 MeshModel::GetcCenter()
{
	double T = vertices_[0][1];
	double L = vertices_[0][0];
	double B = vertices_[0][1];
	double R = vertices_[0][0];
	double F = vertices_[0][2];
	double N = vertices_[0][2];

	for (int i = 0; i < vertices_.size(); i++) {

		glm::vec3 temp_new = vertices_[i];
		if (temp_new[1] >= T)
			T = temp_new[1];
		if (temp_new[1] <= B)
			B = temp_new[1];

		if (temp_new[0] >= L)
			L = temp_new[0];
		if (temp_new[0] <= R)
			R = temp_new[0];

		if (temp_new[2] >= N)
			N = temp_new[2];
		if (temp_new[2] <= F)
			F = temp_new[2];
	}
	glm::vec3 center = glm::vec3((R + L) / 2, (T + B) / 2, (F + N) / 2);
	return center;
}

glm::vec3& MeshModel::getPosition()
{
	return objPosition;
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



