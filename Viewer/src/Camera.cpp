#include "Camera.h"
#include <math.h>       /* sqrt */


Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}

void LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) {
	glm::vec4 z = normalization(eye - at);
	glm::vec4 x = normalization(cross(up,z));
	glm::vec4 y = normalization(cross(z, x));
	glm::vec4 t = glm::vec4(0, 0, 0, 1);
	glm::mat4x4 c = glm::mat4x4(x, y, z, t);
	return c * Translate(-eye);
}
glm::vec4 normalization( glm::vec3& v) {
	float lengh = sqrt(pow(v[0],2)+ pow(v[1], 2)+ pow(v[2], 2));
	v[0] = v[0] / lengh;
	v[1] = v[1] / lengh;
	v[2] = v[2] / lengh;
}
glm::vec4 crossproduct(const glm::vec3& v1, const glm::vec3& v2)

{
	glm::vec3 v3;
	v3[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);

	v3[1] = -((v1[0] * v2[2]) - (v1[2] * v2[0]));

	v3[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
	return v3; 

}



/*
Camera::Camera(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
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


glm::mat4x4 Camera::worldRotationTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);
glm::mat4x4 Camera::worldTranslateTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);
glm::mat4x4 Camera::worldScaleTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);


void Camera::~Camera()
{
}
//get faces of the current model
const Face& Camera::GetFace(int index) const
{
	return faces_[index];
}
//get faces count
int Camera::GetFacesCount() const
{
	return faces_.size();
}

//get model name
const std::string& Camera::GetModelName() const
{
	return model_name_;
}

//get the local transformation 
void Camera::GETlocal() {
	glm::mat4x4 temp = localScaleTransform;
	temp[0][0] += localScaleBarValue;
	temp[1][1] += localScaleBarValue;
	temp[2][2] += localScaleBarValue;
	objectTransform = localTranslateTransform * temp * localRotationTransform;
}
//get the world transfrmation
void Camera::GETworld() {
	worldTransform = worldTranslateTransform * worldScaleTransform * worldRotationTransform;
}


//set Local Rotation Transform by getting alfa parameter (in degrees)
void Camera::LocalRotationTransform(const float alfa) {
	localRotateBarValue = localRotateBarValue + alfa;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);
}
//set World Rotation Transform by getting alfa parameter (in degrees)
void Camera::WorldRotationTransform(const float alfa) {
	worldRotateBarValue += alfa;
	worldRotationTransform[0][0] = cos((worldRotateBarValue * 3.14) / 180);
	worldRotationTransform[0][1] = sin((worldRotateBarValue * 3.14) / 180);
	worldRotationTransform[1][0] = -sin((worldRotateBarValue * 3.14) / 180);
	worldRotationTransform[1][1] = cos((worldRotateBarValue * 3.14) / 180);
}
//set Local Translate Transform by getting x,y,z parameter (parameters which determine the change)
void Camera::LocalTranslateTransform(const float x, const float y, const float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] += y;
	localTranslateTransform[3][2] += z;

}
//set World Translate Transform by getting x,y,z parameter (parameters which determine the change)
void Camera::WorldTranslateTransform(const float x, const float y, const float z) {
	worldTranslateTransform[3][0] += x;
	worldTranslateTransform[3][1] += y;
	worldTranslateTransform[3][2] += z;

}
//set Local Scale Transform by getting x,y,z parameter (parameters which determine the change)
void Camera::LocalScaleTransform(const float x, const float y, const float z)
{
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
}
//set World Scale Transform by getting x,y,z parameter (parameters which determine the change)
void Camera::WorldScaleTransform(const float x, const float y, const float z) {
	worldScaleTransform[0][0] *= x;
	worldScaleTransform[1][1] *= y;
	worldScaleTransform[2][2] *= z;

}


//Accepting the transformation by multiplying the global transformation by the local one
glm::mat4x4 Camera::GetTransform()
{
	GETlocal();
	GETworld();

	return worldTransform * objectTransform;
}

//set modal name
void Camera::SetModelName(std::string name)
{
	model_name_ = name;
}
// set scale bar value
void Camera::SetScaleBarValue(float value)
{
	localScaleBarValue = value;
}

// get scale bar value
float& Camera::GetScaleBarValue()
{
	return localScaleBarValue;
}

// set rotate bar value
void Camera::SetRotateBarValue(float value)
{
	localRotateBarValue = value;

}
*/

