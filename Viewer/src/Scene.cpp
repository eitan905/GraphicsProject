#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "Utils.h"
#include <iostream>

Scene::Scene() :
	active_camera_index_(0),
	active_model_index_(0),
	viewport_width_(1280),
	viewport_height_(720)
{
	

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	if (mesh_model->GetModelName() == "camera.txt" || mesh_model->GetModelName() == "camera.obj") {
		AddCamera(std::make_shared<Camera>(mesh_model->GetFaces(),mesh_model->getVertices(),mesh_model->GetNormals()
			,mesh_model->GetModelName()));
		return;
	}
	
	active_model_index_ = mesh_models_.size();
	mesh_models_.push_back(mesh_model);	

}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models_[index];
}//

light& Scene::GetActiveLight()
{
	return lights[active_light_index_];
}

void Scene::SetActiveLightIndex(int index)
{
	active_light_index_ = index;
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

void Scene::AddLight()
{
	lights.push_back(light(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));
	active_light_index_ = lights.size() - 1;
}
std::vector<light> Scene::GetLights()
{
	return lights;
}

int Scene::GetActiveLightIndex()
{
	return active_light_index_;
}

int Scene::GetLightsCount()
{
	return lights.size();
}



void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	active_camera_index_ = cameras_.size();
	camera->SetModelName("camera " + std::to_string(active_camera_index_ + 1));
	camera->viewport_height_ = viewport_height_;
	camera->viewport_width_ = viewport_width_;
	cameras_.push_back(camera);
	
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

MeshModel& Scene::GetCamera(int index) const
{
	return *cameras_[index];
}

Camera& Scene::GetActiveCamera()
{
	return *cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}
//
void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

Camera& Scene::GetCameraAtIndex(int index) const
{
	return *cameras_[index];
}

glm::mat4x4 Scene::GetPerspectiveTransform(MeshModel& obj, float& z)
{
	glm::vec4 temp;
	Camera camera = GetActiveCamera();
	glm::vec4 frustum = camera.GetFrustum();
	glm::mat4x4 cameraTransform = camera.GetCameraTransform();
	glm::mat4x4 projection = camera.GetProjectionTransformation();
	glm::mat4x4 perspective = camera.GetPerspectiveNormalization();
	return perspective * cameraTransform * obj.GetTransform();
}

glm::mat4x4 Scene::GetOrthographicTransform(MeshModel& obj,float& z)
{
	Camera camera = GetActiveCamera();
	glm::vec4 temp = glm::vec4 (obj.getVerticeAtIndex(2), 1);
	//std::cout <<"before" << temp[2] << std::endl;
	glm::mat4x4 cameraTransform = camera.GetCameraTransform();
	glm::mat4x4 ortho = camera.GetOrthoNormalization();
	temp = cameraTransform * obj.GetTransform() * temp;
	//std::cout <<"after" << temp[2] << std::endl;
	return cameraTransform * obj.GetTransform();
}

glm::mat4x4 Scene::GetProjection(MeshModel& obj, float& z)
{
	Camera camera = GetActiveCamera();
	if (camera.GetActiveProjection() == 1) {
		return GetOrthographicTransform(obj,z);
	}
	else {
		return GetPerspectiveTransform(obj,z);
	}
}

glm::vec3 Scene::HomToCartesian(glm::vec4 vec)
{

	if (vec[3] == 0) {
		return glm::vec3(vec[0], vec[1], vec[2]);

	}
	return glm::vec3(vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]);
}

void Scene::SetWidth(double w)
{
	viewport_width_ = w;
}

void Scene::SetHeight(double h)
{
	viewport_height_ = h;
}

float Scene::GetWidth()
{
	return viewport_width_;
}

float Scene::GetHeight()
{
	return viewport_height_;
}


