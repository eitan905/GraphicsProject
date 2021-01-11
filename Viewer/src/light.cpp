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


light::light(glm::vec3 I, glm::vec3 N, glm::vec3 V, glm::vec3 L_A, glm::vec3 L_D, glm::vec3 L_S,
	std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	MeshModel(faces, vertices, normals, model_name) {
	int alfa;
	this->N = N;
	this->V = V;
	
	this->L_A = L_A;
	this->L_D = L_D;
	this->L_S = L_S;
	localTransform = glm::mat4(1);
	paralel - glm::vec3(1, 1, 1);
	I = paralel;
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

glm::vec3 light::GetPosVec()
{
	return position;
}

float light::GetCosAlpha(glm::vec3 v1, glm::vec3 v2)
{
	float aTimesb = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	float sizeA = pow((pow(v1[0], 2)) + (pow(v1[1], 2)) + (pow(v1[2], 2)), 0.5);
	float sizeB = pow((pow(v2[0], 2)) + (pow(v2[1], 2)) + (pow(v2[2], 2)), 0.5);
	return abs(aTimesb / sizeA * sizeB);



}

void light::SetPos(glm::vec3 new_pos)
{
	position = new_pos;
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
	I_A = glm::normalize(I_A);
}


void light::Find_I_D(glm::vec3 K_D) {
	float alfa;
	alfa = dot(I,N);
	I_D[0] = L_D[0] * K_D[0];
	I_D[1] = L_D[1] * K_D[1];
	I_D[2] = L_D[2] * K_D[2];
	//std::cout << alfa << std::endl;
	this->I_D = Mul(alfa,I_D);
	I_D = glm::normalize(I_D);

}


void light::Find_I_S(glm::vec3 K_S,int user_angle) {
	float alfa;
	alfa = dot(R, V);
	I_S[0] = L_S[0] * K_S[0];
	I_S[1] = L_S[1] * K_S[1];
	I_S[2] = L_S[2] * K_S[2];
	this->I_S = Mul(pow(alfa, user_angle) ,I_S);
	I_S = glm::normalize(I_S);
}


glm::vec3 light::Final_light(glm::vec3 K_A, glm::vec3 K_D,glm::vec3 K_S, int user_angle, glm::vec3 V) {
	glm::vec3 final_color ;
	
	/*K_A = glm::normalize(K_A);

	K_S = glm::normalize(K_S);
	K_D = glm::normalize(K_D);
	L_A = glm::normalize(L_A);
	L_S = glm::normalize(L_S);
	L_D = glm::normalize(L_D);*/
	/*std::cout << K_A[0] << ",";
	std::cout << K_A[1] << ",";
	std::cout << K_A[2] << std::endl;*/
	paralel = glm::normalize(paralel);
	/*std::cout << I[0] << ",";
	std::cout << I[1] << ",";
	std::cout << I[2] << std::endl;*/
	R = glm::normalize(Mul(2, Mul(dot(I,N), N)) - I);
	
	//R = glm::normalize(glm::reflect(-I, N));
	Find_I_S(K_S, user_angle);
	Find_I_A(K_A);
	Find_I_D(K_D);
	
	/*std::cout << I_D[0] << ",";
	std::cout << I_D[1] << ",";
	std::cout << I_D[2] << std::endl;*/
	
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

