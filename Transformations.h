#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Transformations {

public:
	glm::vec3 Scale3(double x, double y, double z, glm::vec3 vector);
	glm::vec3 Transform3(double x, double y, double z, glm::vec3 vector);
	glm::vec4 ToHomogeneous4(glm::vec3 vector);
	glm::vec3 FromHomogeneous4(glm::vec4 vector);










};