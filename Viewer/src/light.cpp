#include "light.h"
#include <math.h>       /* sqrt */
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp> 

//Constractor 

glm::vec3 light::Mul(float x, glm::vec3 vec) {
	return glm::vec3(x * vec[0], x * vec[1], x * vec[2]);
}

float min(int x, int y) {
	return x < y ? x : y;
}
float max(int x, int y) {
	return x > y ? x : y;
}


light::light(glm::vec3 I, glm::vec3 N, glm::vec3 V, glm::vec3 L_A, glm::vec3 L_D, glm::vec3 L_S,
	std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	MeshModel(faces, vertices, normals, model_name),
	L_D(40,50, 110),
	L_S(40,50, 110),
	L_A(40,50, 110)
{
	light_type = "Point";
	int alfa;
	this->N = N;
	this->V = V;
	user_angle = 2;
	
	this->L_A = L_A;
	this->L_D = L_D;
	this->L_S = L_S;
	localTransform = glm::mat4(1);
	paralel - glm::vec3(1, 1, 1);
	I_A = glm::vec3(33, 37, 109);
	I_D = glm::vec3(58, 61, 144);
	I_S = glm::vec3(50, 54, 126);
}

void light::SetTransformX(float val)
{
	localTransform[0][0] += val;
}

void light::SetTransformY(float val)
{
	localTransform[1][1] += val;
}

void light::SetTransformZ(float val)
{
	localTransform[2][2] += val;
}

glm::mat4 light::GetTransform()
{
	glm::mat4 temp(1);
	temp[3][0] = localTransform[0][0];
	temp[3][1] = localTransform[1][1];
	temp[3][2] = localTransform[2][2];
	return temp;
	
}


float light::GetCosAlpha(glm::vec3 v1, glm::vec3 v2)
{
	float aTimesb = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	float sizeA = pow((pow(v1[0], 2)) + (pow(v1[1], 2)) + (pow(v1[2], 2)), 0.5);
	float sizeB = pow((pow(v2[0], 2)) + (pow(v2[1], 2)) + (pow(v2[2], 2)), 0.5);
	return abs(aTimesb / sizeA * sizeB);



}

void light::SetPos(glm::vec3 pos)
{
	position = GetTransform() * glm::vec4(vertices_[0], 1);
}

glm::vec3 light::GetPosVec()
{
	return position;
}


void light::SetActiveLight(std::string light)
{
	light_type = light;
}

glm::vec3 light::GetVerAtIndex(int index)
{
	return vertices_[index];
}

void light::Set_I(glm::vec3 I) {
	this->I = I;
}
void light::Set_N(glm::vec3 N) {
	this->N = N;
}
void light::Set_V(glm::vec3 V) {
	this->V = V;
}
void light::Set_R(glm::vec3 R) {
	this->R = R;
}
void light::Set_L_A(glm::vec3 L_A) {
	this->L_A = L_A;
}
void light::Set_L_D(glm::vec3 L_D) {
	this->L_D = L_D;
}


void light::Set_L_S(glm::vec3 L_S) {
	this->L_S = L_S;
}

void light::Find_I_A(glm::vec3 K_A) {
	//this->I_A = cross(this->L_A,K_A);
	I_A[0] = L_A[0] * K_A[0];
	I_A[1] = L_A[1] * K_A[1];
	I_A[2] = L_A[2] * K_A[2];
	//I_A = glm::cross(L_A, K_A);
	//this->I_A = cross(this->L_A, K_A);
	I_A = glm::normalize(I_A);
}


void light::Find_I_D(glm::vec3 K_D) {
	float alfa;
	alfa = ((dot(N,I)));
	//std::cout << alfa << std::endl;
	I_D[0] = L_D[0] * K_D[0];
	I_D[1] = L_D[1] * K_D[1];
	I_D[2] = L_D[2] * K_D[2];
	//I_D = glm::cross(L_D, K_D);
	I_D = glm::normalize(I_D);
	I_D = Mul(alfa,I_D);

	

}


void light::Find_I_S(glm::vec3 K_S,int user_angle) {
	float alfa;
	//I_S = glm::vec3(0, 0, 0);
	alfa = abs(dot(R,V));
	I_S[0] = L_S[0] * K_S[0];
	I_S[1] = L_S[1] * K_S[1];
	I_S[2] = L_S[2] * K_S[2];
	//I_S = glm::cross(L_S, K_S);
	I_S = glm::normalize(I_S);

	this->I_S = Mul(pow(alfa, user_angle) ,I_S);
}


glm::vec3 light::Final_light(glm::vec3 K_A, glm::vec3 K_D,glm::vec3 K_S, int user_angle, glm::vec3 V) {
	glm::vec3 final_color ;

	if (light_type == "Parallel") {
		I = glm::normalize(paralel);
	}
	I = glm::normalize(I);
	V = glm::normalize(V);
	N = glm::normalize(N);
	R = glm::normalize(glm::reflect(-I, N));
	/*std::cout << I[0] << "," << I[1] << "," << I[2] << std::endl;
	std::cout << V[0] << "," << V[1] << "," <<V[2] << std::endl;
	std::cout << N[0] << "," << N[1] << "," << N[2] << std::endl;
	std::cout << R[0] << "," << R[1] << "," << R[2] << std::endl;*/
	Find_I_S(K_S, this->user_angle);
	Find_I_A(K_A);
	Find_I_D(K_D);
	


	final_color = this->I_S + this->I_D + this->I_A;
	// = glm::normalize(final_color);
	if (final_color[0] > 1) {
		final_color[0] = 1;
	}
	if (final_color[1] > 1) {
		final_color[1] = 1;
	}
	if (final_color[2] > 1) {
		final_color[2] = 1;
	}

	if (final_color[0] < 0) {
		final_color[0] = 0;
	}
	if (final_color[1] < 0) {
		final_color[1] = 0;
	}
	if (final_color[2] < 0) {
		final_color[2] = 0;
	}

	/*final_color[0] = (final_color[0]+1)/2;
	final_color[1] = (final_color[1]+1)/2;
	final_color[2] = (final_color[2]+1)/2;*/

	//if (final_color[0] < 0 || final_color[1] < 0 || final_color[2] < 0) {
	//	std::cout << "----------------------------------------------" << std::endl;
	//	std::cout << final_color[0] << "," << final_color[1] << "," << final_color[2] << std::endl;;
	//	/*std::cout << I_A[0] << "," << I_A[1] << "," << I_A[2] << std::endl;
	//	std::cout << I_D[0] << "," << I_D[1] << "," << I_D[2] << std::endl;
	//	std::cout << I_S[0] << "," << I_S[1] << "," << I_S[2] << std::endl;*/
	//}

	return final_color;

}



void light::Find_I_A_gouraud(glm::vec3 K_A) {
	I_A[0] = L_A[0] * K_A[0];
	I_A[1] = L_A[1] * K_A[1];
	I_A[2] = L_A[2] * K_A[2];
	
}


void light::Find_I_D_gouraud(glm::vec3 K_D) {
	float alfa;
	alfa = ((dot(I, N)));
	I_D[0] = L_D[0] * K_D[0];
	I_D[1] = L_D[1] * K_D[1];
	I_D[2] = L_D[2] * K_D[2];

	I_D = Mul(alfa, I_D);

}

void light::Find_I_S_gouraud(glm::vec3 K_S, int user_angle) {
	float alfa;
	alfa = abs(dot(R, V));

	I_S[0] = L_S[0] * K_S[0];
	I_S[1] = L_S[1] * K_S[1];
	I_S[2] = L_S[2] * K_S[2];

	this->I_S = Mul(pow(alfa, user_angle), I_S);
}

glm::vec3 light::Final_light_gouraud(glm::vec3 K_A, glm::vec3 K_D, glm::vec3 K_S) {
	glm::vec3 final_color;

	if (light_type == "Parallel") {
		I = glm::normalize(paralel);
	}
	I = glm::normalize(I);
	V = glm::normalize(V);
	N = glm::normalize(N);
	R = glm::normalize(glm::reflect(-I, N));
	Find_I_S(K_S, this->user_angle);
	Find_I_A(K_A);
	Find_I_D(K_D);
	
	

	final_color = this->I_S + this->I_D + this->I_A;
	if (final_color[0] > 1) {
		final_color[0] = 1;
	}
	if (final_color[1] > 1) {
		final_color[1] = 1;
	}
	if (final_color[2] > 1) {
		final_color[2] = 1;
	}

	return final_color;

}
