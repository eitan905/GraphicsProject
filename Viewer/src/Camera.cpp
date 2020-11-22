#include "Camera.h"
#include <math.h>       /* sqrt */


Camera::~Camera()
{
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
glm::vec4 Camera::crossproduct(const glm::vec4& v1, const glm::vec4& v2) {
	glm::vec3 vec1 = glm::vec3(v1[0], v1[1], v1[2]);
	glm::vec3 vec2 = glm::vec3(v2[0], v2[1], v2[2]);
	glm::vec3 res = cross(vec1, vec2);
	return glm::vec4(res[0], res[1], res[2], 1);
}
glm::mat4x4 Camera::LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) {
	glm::vec4 z = normalize(eye - at);
	glm::vec4 x = normalize(crossproduct(up,z));
	glm::vec4 y = normalize(crossproduct(z, x));
	glm::vec4 t = glm::vec4(0, 0, 0, 1);
	glm::mat4x4 c = glm::mat4x4(x, y, z, t);
	Translate(-eye);
	c = c * localTranslateTransform;
	cinv =glm::inverse(localTranslateTransform) *glm::inverse(c);

	for (int i = 0; i < 4; i++) {
		//DrawModel(scene.GetModel(i));
	}
	
	return cinv;
} 

void Camera::Translate(const glm::vec4& v) {
	localTranslateTransform[3][0] = v[0];
	localTranslateTransform[3][1] = v[1];
	localTranslateTransform[3][2] = v[2];
}
void Camera::TranslateSpace(float x,float y, float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] +=y;
	localTranslateTransform[3][2] +=z;
	c = c * localTranslateTransform;
	cinv = glm::inverse(localTranslateTransform) * glm::inverse(c);
}

void Camera::TranslateWorld(float x, float y, float z) {
	worldTranslateTransform[3][0] += x;
	worldTranslateTransform[3][1] += y;
	worldTranslateTransform[3][2] += z;
	c = c * worldTranslateTransform;
	cinv = glm::inverse(worldTranslateTransform) * glm::inverse(c);
}

void Camera::ScaleWorld(float x, float y, float z) {
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
	c = c * worldScaleTransform;
	cinv = glm::inverse(worldScaleTransform) * glm::inverse(c);
}
void Camera::ScaleLocal(float x, float y, float z) {
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
	c = c * localScaleTransform;
	cinv = glm::inverse(localScaleTransform) * glm::inverse(c);
}

void Camera::RotateWorld(float x) {
	localRotateBarValue = localRotateBarValue + x;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);
	c = c * worldRotationTransform;
	cinv = glm::inverse(worldRotationTransform) * glm::inverse(c);
}

void Camera::RotateLocal(float x) {
	localRotateBarValue = localRotateBarValue + x;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);
	c = c * localRotationTransform;
	cinv = glm::inverse(localRotationTransform) * glm::inverse(c);
}


void Camera::TranslatLocal(float x, float y, float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] += y;
	localTranslateTransform[3][2] += z;
	c = c * localTranslateTransform;
	cinv = glm::inverse(localTranslateTransform) * glm::inverse(c);
}



/*
Camera::Camera()
	

{
	//localRotateBarValue = 0;
	//localScaleBarValue = 1;

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

//get faces count

//get model name

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


// set rotate bar value
void Camera::SetRotateBarValue(float value)
{
	localRotateBarValue = value;

}
*/


