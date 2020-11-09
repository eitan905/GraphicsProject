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
			cos((rotateBarValue * 3.14) / 180), sin((rotateBarValue * 3.14) / 180), 0, 0,
			-sin((rotateBarValue * 3.14) / 180), cos((rotateBarValue * 3.14) / 180), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	translateTransform = glm::mat4x4(
		1, 0, 0, translateBarValueX,
		0, 1, 0, translateBarValueY,
		0, 0, 1, translateBarValueZ,
		0, 0, 0, 1
	);
	ScaleTransform = glm::mat4x4(
		scaleValueX, 0, 0, 0,
		0, scaleValueY, 0, 0,
		0, 0, scaleValueZ, 0,
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
			 cos((WrotateBarValue * 3.14) / 180), sin((WrotateBarValue * 3.14) / 180), 0, 0,
			 -sin((WrotateBarValue * 3.14) / 180), cos((WrotateBarValue * 3.14) / 180), 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		 );
	 WtranslateTransform = glm::mat4x4(
		 1, 0, 0, WtranslateBarValueX,
		 0, 1, 0, WtranslateBarValueY,
		 0, 0, 1, WtranslateBarValueZ,
		 0, 0, 0, 1
	 );
	 WScaleTransform = glm::mat4x4(
		 WscaleValueX, 0, 0, 0,
		 0, WscaleValueY, 0, 0,
		 0, 0, WscaleValueZ, 0,
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
	objectTransform = ScaleTransform * translateTransform * rotationTransform;
}

void MeshModel::GETworld() {
	 worldTransform= WScaleTransform * WtranslateTransform * WrotationTransform;
}

glm::mat4x4 MeshModel::GETMAT() {
	GETlocal();
	GETworld();
	return worldTransform * objectTransform;
}
void MeshModel::setRotationTransfromLOCAL(float&alfa) {
	rotateBarValue = alfa;

}

void MeshModel::setRotationTransfromWORLD(float&alfa) {
	WrotateBarValue = alfa;
}

void MeshModel::setTranslateTransfromLOCAL(float& x, float& y, float& z) {
	translateBarValueX = x;
	translateBarValueY = y;
	translateBarValueZ = z;

}

void MeshModel::setTranslateTransfromWORLD(float& x, float& y, float& z) {
	WtranslateBarValueX = x;
	WtranslateBarValueY = y;
	WtranslateBarValueZ = z;

}
void MeshModel::setScaleTransfromWORLD(float& x, float& y, float& z) {
	scaleValueX = x;
	scaleValueY = y;
	scaleValueZ = z;

}
void MeshModel::setScaleTransfromWORLD(float& x, float& y, float& z) {
	WscaleValueX = x;
	WscaleValueY = y;
	WscaleValueZ = z;

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


