#include "Camera.h"
#include <math.h>       /* sqrt */
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	MeshModel(faces, vertices, normals, model_name)
{
	eye = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	at = glm::vec3(0, 0, 0);
	
	distance = 10;
	//camera mat help
	// camera projection_transformation_
	viewport_width_ = float(1280);
	viewport_height_ =float (720);
	fovy = -55.0f;
	aspect = viewport_width_/viewport_height_;
	zNear = 30.0f;
	zFar = 400.0f;
	right = (viewport_width_);
	left = 0.0f;
	bottom = 0.0f;
	top = (viewport_height_);
	activeProjection = 0;
	scaleBarTransform = 0;
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

	worldRotationTransform_X = worldRotationTransform_Y = worldRotationTransform_Z =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	localRotateBarValue_X =
		localRotateBarValue_Y =
		localRotateBarValue_Z =
		worldRotateBarValue_X =
		worldRotateBarValue_Y =
		worldRotateBarValue_Z =
		localTranslateBarValue_Z =
		localTranslateBarValue_X =
		localTranslateBarValue_Y =
		worldScaleBarValue =
		worldTranslateBarValue_Z =
		worldTranslateBarValue_Y =
		worldTranslateBarValue_X = 0;
	localScaleBarValue = 1;

	worldRotationTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	worldScaleTransform =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	worldTranslateTransform =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
}
//world mat


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

glm::vec3 HomToCartesian(glm::vec4 vec)
{

	if (vec[3] == 0) {
		return glm::vec3(vec[0], vec[1], vec[2]);

	}
	return glm::vec3(vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]);
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
	c = localTranslateTransform;
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
	cinv = glm::inverse(localTranslateTransform) * cinv;
	localTranslateTransform = glm::mat4(1);
}

//camera world translate giving 3 number , changing c-invers
void Camera::TranslateWorld(float x, float y, float z) {
	worldTranslateTransform[3][0] += x;
	worldTranslateTransform[3][1] += y;
	worldTranslateTransform[3][2] += z;
	c = c * worldTranslateTransform;
	cinv = glm::inverse(worldTranslateTransform) * cinv;
}

//camera world scale giving 3 number , changing c-invers
void Camera::ScaleWorld(float x, float y, float z) {
	worldScaleTransform[0][0] = x;
	worldScaleTransform[1][1] = y;
	worldScaleTransform[2][2] = z;
	c =  worldScaleTransform * c;
	cinv = cinv * glm::inverse(worldScaleTransform) ;
}

//camera local scale giving 3 number , changing c-invers
void Camera::ScaleLocal(float x, float y, float z) {
	localScaleTransform[0][0] *= x;
	localScaleTransform[1][1] *= y;
	localScaleTransform[2][2] *= z;
	c =  localScaleTransform * c;
	cinv = cinv * glm::inverse(localScaleTransform);
}

//camera rotate world giving one number , changing c-invers
void Camera::RotateWorld(float x) {
	/*localRotateBarValue = localRotateBarValue + x;
	localRotationTransform[0][0] = cos((localRotateBarValue * 3.14) / 180);
	localRotationTransform[0][1] = sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][0] = -sin((localRotateBarValue * 3.14) / 180);
	localRotationTransform[1][1] = cos((localRotateBarValue * 3.14) / 180);*/
	c = worldRotationTransform * c;
	cinv = cinv * glm::inverse(worldRotationTransform);
}
glm::mat4x4 Camera::GetTransform()
{
	UpdateObjcetTransform();
	c = c * glm::lookAt(eye, at, up);
	return c;
}

//camera local rotate giving one number , changing c-invers
void Camera::RotateLocal(float x) {
	LocalRotationTransform_Z();
	LocalRotationTransform_Y();
	LocalRotationTransform_X();
	c = localRotationTransform_X *
		localRotationTransform_Y * localRotationTransform_Z * c;
	cinv = cinv * glm::inverse(localRotationTransform_X *
		localRotationTransform_Y * localRotationTransform_Z);
}

//camera TranslatLocal giving 3 number , changing c-invers
void Camera::TranslatLocal(float x, float y, float z) {
	localTranslateTransform[3][0] += x;
	localTranslateTransform[3][1] += y;
	localTranslateTransform[3][2] += z;
	c = localTranslateTransform * c;
	cinv = cinv * glm::inverse(localTranslateTransform);
	localTranslateTransform = glm::mat4(1);
}

//camera get transform
glm::mat4x4 Camera::GetCameraTransform()
{
	UpdateObjcetTransform();
	
	c = glm::inverse(glm::lookAt(eye, at, up)) * c;
	cinv = cinv * glm::lookAt(eye, at, up);
	
	return cinv;
}

void Camera::UpdateObjcetTransform() {
	LocalRotationTransform_Z();
	LocalRotationTransform_Y();
	LocalRotationTransform_X();
	SetTranslationTransform();
	glm::mat4x4 temp2 = worldTranslateTransform;
	temp2[3][0] += worldTranslateBarValue_X;
	temp2[3][1] += worldTranslateBarValue_Y;
	temp2[3][2] += worldTranslateBarValue_Z;
	glm::mat4x4 temp = localScaleTransform;
	temp[1][1] = localScaleBarValue;
	temp[0][0] = localScaleBarValue;
	temp[2][2] = localScaleBarValue;
	
	c = c * (temp * temp2);
	cinv = glm::inverse(temp * temp2) * cinv;
	WorldRotationTransform_X();
	WorldRotationTransform_Y();
	WorldRotationTransform_Z();
	
	
}


//camera set distance
void Camera::SetDistance(double value)
{
	distance += value;
	zNear = distance + localTranslateBarValue_Z;
	
	
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
	//return glm::perspective(fovy, aspect, zNear, zFar);
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
	return glm::vec3((vec[0] + 1.0f) * (width / 2), (vec[1] + 1.0f) * (height / 2), (vec[2]));
}

glm::vec4 Camera::GetFrustum()
{
	return glm::vec4(fovy, aspect, zNear, zFar);
}
//
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

void Camera::SetFrustum(glm::vec4 tempFrus)
{
	fovy = tempFrus[0];
	aspect = tempFrus[1];
	zNear = tempFrus[2];
	zFar = tempFrus[3];
}

void Camera::SetTranslationTransform()
{
	localTranslateTransform[3][0] = localTranslateBarValue_X;
	localTranslateTransform[3][1] = localTranslateBarValue_Y;
	localTranslateTransform[3][2] = localTranslateBarValue_Z;
	c = c * localTranslateTransform;
	cinv = glm::inverse(localTranslateTransform) * cinv;
	localTranslateTransform = glm::mat4(1);
}

void Camera::SetLookAt(MeshModel& obj)
{
	glm::mat4x4 temp;
	if (activeProjection == 1) {
		temp =  GetOrthoNormalization();
	}
	else {
		temp = GetPerspectiveNormalization();
	}

	glm::vec3 at = obj.GetcCenter();
	glm::mat4x4 projection = projection_transformation_ * temp *cinv* obj.GetTransform();
	at = GetViewPortTransformation(HomToCartesian(projection * glm::vec4(at, 1)),viewport_width_,viewport_height_);
	//at = HomToCartesian(obj.GetTransform() * glm::vec4(at, 1));
	eye = GetcCenter();
	eye = HomToCartesian(c * glm::vec4(eye, 1));
	eye = ::glm::vec3(0, 0, -1);
	//at = glm::vec3(0, 0, 0);
	cinv = glm::lookAt(eye, at, up);
	c = glm::inverse(cinv);
	this->Reset();

}




void Camera::LocalRotationTransform_Z() {
	localRotationTransform_Z[0][0] = cos((localRotateBarValue_Z * 3.14) / (2*180));
	localRotationTransform_Z[0][1] = sin((localRotateBarValue_Z * 3.14) / (2 * 180));
	localRotationTransform_Z[1][0] = -sin((localRotateBarValue_Z * 3.14) / (2 * 180));
	localRotationTransform_Z[1][1] = cos((localRotateBarValue_Z * 3.14) / (2 * 180));
	c = c * localRotationTransform_Z;
	cinv = glm::inverse(localRotationTransform_Z) * cinv;
}
void Camera::Reset()
{
	eye = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);

	distance = 300;
	//camera mat help
	// camera projection_transformation_
	viewport_width_ = float(1280);
	viewport_height_ = float(720);
	fovy = -55.0f;
	aspect = viewport_width_ / viewport_height_;
	zNear = 300.0f;
	zFar = 800.0f;
	right = (viewport_width_);
	left = 0.0f;
	bottom = 0.0f;
	top = (viewport_height_);
	activeProjection = 0;
	scaleBarTransform = 0;
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
	
	//cinv mat
	
	//local transformation

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

	worldRotationTransform_X = worldRotationTransform_Y = worldRotationTransform_Z =
		glm::mat4x4(
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



	localRotateBarValue_X =
		localRotateBarValue_Y =
		localRotateBarValue_Z =
		worldRotateBarValue_X =
		worldRotateBarValue_Y =
		worldRotateBarValue_Z =
		localScaleBarValue =
		localTranslateBarValue_Z =
		localTranslateBarValue_X =
		localTranslateBarValue_Y =
		worldScaleBarValue =
		worldTranslateBarValue_Z =
		worldTranslateBarValue_Y =
		worldTranslateBarValue_X = 0;

}
void Camera::LocalRotationTransform_Y() {
	localRotationTransform_Y[0][0] = cos((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[0][2] = sin((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[2][0] = -sin((localRotateBarValue_Y * 3.14) / (2 * 180));
	localRotationTransform_Y[2][2] = cos((localRotateBarValue_Y * 3.14) / (2 * 180));
	c = c * localRotationTransform_Y;
	cinv = glm::inverse(localRotationTransform_Y) * cinv;
}
void Camera::LocalRotationTransform_X() {
	localRotationTransform_X[1][1] = cos((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[1][2] = sin((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[2][1] = -sin((localRotateBarValue_X * 3.14) / (2 * 180));
	localRotationTransform_X[2][2] = cos((localRotateBarValue_X * 3.14) / (2 * 180));
	c = c * localRotationTransform_X;
	cinv = glm::inverse(localRotationTransform_X) * cinv;
}


void Camera::WorldRotationTransform_Z() {
	worldRotationTransform_Z[0][0] = cos((worldRotateBarValue_Z * 3.14) / (2 * 180));
	worldRotationTransform_Z[0][1] = sin((worldRotateBarValue_Z * 3.14) / (2 * 180));
	worldRotationTransform_Z[1][0] = -sin((worldRotateBarValue_Z * 3.14) / (2 * 180));
	worldRotationTransform_Z[1][1] = cos((worldRotateBarValue_Z * 3.14) / (2 * 180));

	c = worldRotationTransform_Z * c ;
	cinv = cinv * glm::inverse(worldRotationTransform_Z);
}

void Camera::WorldRotationTransform_Y() {
	worldRotationTransform_Y[0][0] = cos((worldRotateBarValue_Y * 3.14) / (2 * 180));
	worldRotationTransform_Y[0][2] = sin((worldRotateBarValue_Y * 3.14) / (2 * 180));
	worldRotationTransform_Y[2][0] = -sin((worldRotateBarValue_Y * 3.14) / (2 * 180));
	worldRotationTransform_Y[2][2] = cos((worldRotateBarValue_Y * 3.14) / (2 * 180));
	c = worldRotationTransform_Y * c;
	cinv = cinv * glm::inverse(worldRotationTransform_Y);
}

void Camera::WorldRotationTransform_X() {
	worldRotationTransform_X[1][1] = cos((worldRotateBarValue_X * 3.14) / (2 * 180));
	worldRotationTransform_X[1][2] = sin((worldRotateBarValue_X * 3.14) / (2 * 180));
	worldRotationTransform_X[2][1] = -sin((worldRotateBarValue_X * 3.14) / (2 * 180));
	worldRotationTransform_X[2][2] = cos((worldRotateBarValue_X * 3.14) / (2 * 180));
	c = worldRotationTransform_X * c;
	cinv = cinv * glm::inverse(worldRotationTransform_X);
}