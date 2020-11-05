#include "Transformations.h"

 glm::vec3 Transformations::Scale3(double x, double y, double z, glm::vec3 vector)
{
	glm::mat3 temp(
		x, 0, 0,
		0, y, 0,
		0, 0, z);
	vector = temp * vector;
	return vector;
}

 glm::vec3  Transformations::Transform3(double x, double y, double z, glm::vec3 vector)
{
	glm::mat4 transform(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);

	glm::vec4 temp = ToHomogeneous4(vector);
	
	temp = transform * temp;

	vector = FromHomogeneous4(temp);
	return vector;
}

 glm::vec4 Transformations::ToHomogeneous4(glm::vec3 vector)
{
	return  (glm::vec4(vector[0], vector[1], vector[2], 1));

}

 glm::vec3 Transformations::FromHomogeneous4(glm::vec4 vector)
{

	glm::vec3 temp(vector[0] / vector[4], vector[1] / vector[4], vector[2] / vector[4]);
	return temp;
}
