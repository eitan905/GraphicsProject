#include "Camera.h"
#include <math.h>       /* sqrt */
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera()
{
	distance = 30;
	//camera mat help
	// camera projection_transformation_
	viewport_width_ = float(1280);
	viewport_height_ =float (720);
	fovy = -55.0f;
	aspect = float(1280 / 720);
	zNear = 30.0f;
	zFar = 530.0f;
	right = (viewport_width_);
	left = 0.0f;
	bottom = 0.0f;
	top = (viewport_height_);
	activeProjection = 0;
	scaleBarTransform = 0;
	
	projection_transformation_ =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 1,
			0, 0, 0, 0
		);
	objectTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	c = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	//cinv mat
	cinv = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	//local transformation
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

	
}
//world mat
glm::mat4x4 Camera::worldRotationTransform = glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);
glm::mat4x4 Camera::worldScaleTransform =
glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);
glm::mat4x4 Camera::worldTranslateTransform = 
glm::mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

Camera::~Camera(){}
// get camera projection
const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	glm::mat4x4 temp = projection_transformation_;
	return (projection_transformation_);
	if (distance != 0) {
		temp[2][3] /= distance;
	}
	return (temp);
}
//get camera view pransform
const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}

//cross product of two 4-vec
glm::vec4 Camera::crossproduct(const glm::vec4& v1, const glm::vec4& v2) {
	glm::vec3 vec1 = glm::vec3(v1[0], v1[1], v1[2]);
	glm::vec3 vec2 = glm::vec3(v2[0], v2[1], v2[2]);
	glm::vec3 res = cross(vec1, vec2);
	return glm::vec4(res[0], res[1], res[2], 1);
}

//camera lookat func - the begining just in case
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
		//////////////////////////////////
	}
	
	return cinv;
} 

//camera translate, giving 4-vec
void Camera::Translate(const glm::vec4& v) {
	localTranslateTransform[3][0] = v[0];
	localTranslateTransform[3][1] = v[1];
	localTranslateTransform[3][2] = v[2];
}

//camera translate giving 3 number , changing c-invers
void Camera::TranslateSpace(float x,float y, float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] +=y;
	localTranslateTransform[3][2] +=z;
	c = c * localTranslateTransform;
	cinv = glm::inverse(localTranslateTransform) * glm::inverse(c);
}

//camera world translate giving 3 number , changing c-invers
void Camera::TranslateWorld(float x, float y, float z) {
	worldTranslateTransform[3][0] += x;
	worldTranslateTransform[3][1] += y;
	worldTranslateTransform[3][2] += z;
	c = c * worldTranslateTransform;
	cinv = glm::inverse(worldTranslateTransform) * glm::inverse(c);
}

//camera world scale giving 3 number , changing c-invers
void Camera::ScaleWorld(float x, float y, float z) {
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
	c = c * worldScaleTransform;
	cinv = glm::inverse(worldScaleTransform) * glm::inverse(c);
}

//camera local scale giving 3 number , changing c-invers
void Camera::ScaleLocal(float x, float y, float z) {
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
	c = c * localScaleTransform;
	cinv = glm::inverse(localScaleTransform) * glm::inverse(c);
}

//camera rotate world giving one number , changing c-invers
void Camera::RotateWorld(float x) {
	localRotateBarValue = localRotateBarValue + x;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);
	c = c * worldRotationTransform;
	cinv = glm::inverse(worldRotationTransform) * glm::inverse(c);
}

//camera local rotate giving one number , changing c-invers
void Camera::RotateLocal(float x) {
	localRotateBarValue = localRotateBarValue + x;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);
	c = c * localRotationTransform;
	cinv = glm::inverse(localRotationTransform) * glm::inverse(c);
}

//camera TranslatLocal giving 3 number , changing c-invers
void Camera::TranslatLocal(float x, float y, float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] += y;
	localTranslateTransform[3][2] += z;
	c = c * localTranslateTransform;
	cinv = glm::inverse(localTranslateTransform) * glm::inverse(c);
}

//camera get transform
glm::mat4x4 Camera::GetTransform()
{
	glm::mat4x4 temp = localTranslateTransform;
	temp[2][2] += scaleBarTransform;
	return glm::inverse( temp
		* localScaleTransform * localRotationTransform);
}

//camera set distance
void Camera::SetDistance(double value)
{
	distance += value;
	zNear = distance;
	zFar = distance + 500;
	
}

//camera get OrthoNormalization matrix
glm::mat4x4 Camera::GetOrthoNormalization() {

	glm::mat4x4 temp(
		2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, 2 / (zNear - zFar), 0,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), 1
	);
	return (temp);
}

//camera get PerspectiveNormalization matrix
glm::mat4x4 Camera::GetPerspectiveNormalization() {

	//return (glm::frustum(left, right, bottom, top, near, far));
	//float const rad = fovy;

    float tanHalfFovy = tan(float((3.14*fovy)/180) / 2.0f);

	glm::mat4x4 temp(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
    temp[0][0] = 1 / (aspect * tanHalfFovy);
	temp[1][1] = 1 / (tanHalfFovy);
	temp[2][2] = - (zFar + zNear) / (zFar - zNear);
	temp[2][3] = - 1;
	temp[3][2] = - (2.0f * zFar * zNear) / (zFar - zNear);

	/*glm::mat4x4 temp1(
		2*near / (right - left), 0, 0, 0,
		0, 2*near/ (top - bottom), 0, 0,
		(right+left)/(right-left), (top+bottom)/(top-bottom), -(far+near)/(far-near), -1,
		0,0, -2 * near*far / (far-near),0
	);*/

	return (temp);

}

//Get View PortTransformation camera vec3
glm::vec3 Camera::GetViewPortTransformation(glm::vec3 vec,float width,float height) {
	return glm::vec3((vec[0] + 1.0f) * (width / 2), (vec[1] + 1.0f) * (height / 2), vec[3]);
}

glm::vec4 Camera::GetFrustum()
{
	return glm::vec4(fovy, aspect, distance, distance + 500);
}

int Camera::GetActiveProjection()
{
	return activeProjection;
}
void Camera::SetActiveProjection(bool value)
{
	if (value == true) {
		activeProjection = (activeProjection + 1) % 2;
	}
}

float& Camera::GetScaleBarValue()
{
	return scaleBarTransform;
}


void Camera::SetScaleBarValue(float value)
{
	scaleBarTransform  = value;
}

