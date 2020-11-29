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
	if (mesh_model->GetModelName() == "camera.txt") {
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

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	active_camera_index_ = cameras_.size();
	cameras_.push_back(camera);
	
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

Camera& Scene::GetCamera(int index)
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

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

glm::mat4x4 Scene::GetPerspectiveTransform()
{
	Camera camera = GetActiveCamera();
	MeshModel obj = GetActiveModel();
	glm::vec4 frustum = camera.GetFrustum();
	glm::mat4x4 cameraTransform = camera.GetCameraTransform();
	glm::mat4x4 projection = camera.GetProjectionTransformation();
	glm::mat4x4 perspective = camera.GetPerspectiveNormalization();
	return projection * perspective * cameraTransform * obj.GetTransform();
}

glm::mat4x4 Scene::GetOrthographicTransform()
{
	Camera camera = GetActiveCamera();
	MeshModel obj = GetActiveModel();
	glm::mat4x4 cameraTransform = camera.GetCameraTransform();
	glm::mat4x4 ortho = camera.GetOrthoNormalization();
	return ortho * cameraTransform * obj.GetTransform();
}

glm::mat4x4 Scene::GetProjection()
{
	Camera camera = GetActiveCamera();
	if (camera.GetActiveProjection() == 1) {
		return GetOrthographicTransform();
	}
	else {
		return GetPerspectiveTransform();
	}
}




