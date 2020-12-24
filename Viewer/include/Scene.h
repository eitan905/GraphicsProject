#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"

class Scene {
public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const std::shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	MeshModel& GetCamera(int index) const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	Camera& GetCameraAtIndex(int index) const;
	
	glm::mat4x4 GetPerspectiveTransform(MeshModel& obj, float& z);
	glm::mat4x4 GetOrthographicTransform(MeshModel& obj,float& z);
	glm::mat4x4 GetProjection(MeshModel& obj,float& z);
	glm::vec3 HomToCartesian(glm::vec4 vec);
	void SetWidth(double w);
	void SetHeight(double h);
	float GetWidth();
	float GetHeight();
	bool displayBox;
	bool displayNormals;

	
	
private:
	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<std::shared_ptr<Camera>> cameras_;

	float viewport_width_;
	float viewport_height_;
	
	int active_camera_index_;
	int active_model_index_;
	
};