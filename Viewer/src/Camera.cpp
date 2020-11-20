#include "Camera.h"
#include <math.h>       /* sqrt */

Camera::Camera()
{
	
}

Camera::~Camera()
{
	localTranslateTransform =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}
glm::vec4 crossproduct(const glm::vec4& v1, const glm::vec4& v2) {
	glm::vec3 vec1 = glm::vec3(v1[0], v1[1], v1[2]);
	glm::vec3 vec2 = glm::vec3(v2[0], v2[1], v2[2]);
	glm::vec3 res = cross(vec1, vec2);
	return glm::vec4(res[0], res[1], res[2], 1);
}
void LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) {
	glm::vec4 z = normalize(eye - at);
	glm::vec4 x = normalize(crossproduct(up,z));
	glm::vec4 y = normalize(crossproduct(z, x));
	glm::vec4 t = glm::vec4(0, 0, 0, 1);
	glm::mat4x4 c = glm::mat4x4(x, y, z, t);
	Translate(-eye);
	c = c * localTranslateTransform;
	cinv =glm::inverse(localTranslateTransform) *glm::inverse(c);
	return c;
}

void Translate(const glm::vec4& v) {
	localTranslateTransform[3][0] = v[0];
	localTranslateTransform[3][1] = v[1];
	localTranslateTransform[3][2] = v[2];
}
void TranslateSpace(float x,float y, float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] +=y;
	localTranslateTransform[3][2] +=z;
	c = c * localTranslateTransform;
	cinv = glm::inverse(localTranslateTransform) * glm::inverse(c)
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

