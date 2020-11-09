#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)

{
	rotateBarValue = 0;
	scaleBarValue = 1;
	translateBarValueX = 0;
	translateBarValueY = 0;
	translateBarValueZ = 0;
	scaleValueX = 1;
	scaleValueY = 1;
	scaleValueZ = 1;
	objectTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	rotationTransform =
		glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	translateTransform = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	ScaleTransform = glm::mat4x4(
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

	 WrotateBarValue = 0;
	 WtranslateBarValueX = 0;
	 WtranslateBarValueY = 0;
	 WtranslateBarValueZ = 0;
	 WscaleValueX = 1;
	 WscaleValueY = 1;
	 WscaleValueZ = 1;
	 
	 WrotationTransform =
			 glm::mat4x4(
				 1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, 1, 0,
				 0, 0, 0, 1
			 );
	 WtranslateTransform = glm::mat4x4(
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	 );
	 WScaleTransform = glm::mat4x4(
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	 );
}
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


void MeshModel::GETlocal() {
	objectTransform = translateTransform * ScaleTransform * rotationTransform;
}

void MeshModel::GETworld() {
	 worldTransform = WtranslateTransform * WScaleTransform * WrotationTransform;
}

glm::mat4x4 MeshModel::GETMAT() {
	GETlocal();
	GETworld();
	glm::mat4x4 temp = objectTransform * rotationTransform;
	temp[0][0] += scaleBarValue;
	temp[1][1] += scaleBarValue;
	temp[2][2] += scaleBarValue;
	return worldTransform * temp;
}
void MeshModel::setRotationTransfromLOCAL(const float alfa) {
	rotateBarValue += alfa;
	rotationTransform[0][0] = cos((rotateBarValue * 3.14) / 180);
	rotationTransform[0][1] = sin((rotateBarValue * 3.14) / 180);
	rotationTransform[1][0] = -sin((rotateBarValue * 3.14) / 180);
	rotationTransform[1][1] = cos((rotateBarValue * 3.14) / 180);
}

void MeshModel::setRotationTransfromWORLD(const float alfa) {
	WrotateBarValue += alfa;
	WrotationTransform[0][0] = cos((WrotateBarValue * 3.14) / 180);
	WrotationTransform[0][1] = sin((WrotateBarValue * 3.14) / 180);
	WrotationTransform[1][0] = -sin((WrotateBarValue * 3.14) / 180);
	WrotationTransform[1][1] = cos((WrotateBarValue * 3.14) / 180);
}

void MeshModel::setTranslateTransfromLOCAL(const float x, const float y, const float z) {
	translateTransform[3][0] += x;
	translateTransform[3][1] += y;
	translateTransform[3][2] += z;

}

void MeshModel::setTranslateTransfromWORLD(const float x, const float y, const float z) {
	WtranslateTransform[3][0] += x;
	WtranslateTransform[3][1] += y;
	WtranslateTransform[3][2] += z;

}
void MeshModel::setScaleTransfromLOCAL(const float x, const float y, const float z)
{
	ScaleTransform[0][0] *= x;
	ScaleTransform[1][1] *= y;
	ScaleTransform[2][2] *= z;
}
void MeshModel::setScaleTransfromWORLD(const float x, const float y, const float z) {
	WScaleTransform[0][0] *= x;
	WScaleTransform[1][1] *= y;
	WScaleTransform[2][2] *= z;

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


