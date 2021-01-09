#include "light.h"
#include <math.h>       /* sqrt */
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Constractor 

glm::vec3 light::Mul(int x, glm::vec3 vec) {
	return glm::vec3(x * vec[0], x * vec[1], x * vec[2]);
}


light::light(glm::vec3 I, glm::vec3 N, glm::vec3 V, glm::vec3 L_A, glm::vec3 L_D, glm::vec3 L_S) {
	int alfa;
	this->I = I;
	this->N = N;
	this->V = V;
	alfa = dot(I,N);
	this->R = Mul(2,Mul(alfa,N))-I;
	this->L_A = L_A;
	this->L_D = L_D;
	this->L_S = L_S;
	localTransform = glm::mat4(1);
}

void light::SetTransformX(float val)
{
	localTransform[0][0] = val;
}

void light::SetTransformY(float val)
{
	localTransform[1][1] = val;
}

void light::SetTransformZ(float val)
{
	localTransform[2][2] = val;
}

glm::mat4 light::GetTransform()
{
	return localTransform;
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
	this->I_A = cross(this->L_A,K_A);
}


void light::Find_I_D(glm::vec3 K_D) {
	int alfa;
	alfa= dot(this->I, this->N);
	this->I_D = Mul(alfa,cross(this->L_D, K_D));
}


void light::Find_I_S(glm::vec3 K_S,int user_angle) {
	int alfa;
	alfa = dot(this->R, this->V);
	this->I_S = Mul(pow(alfa, user_angle) ,cross(this->L_S, K_S));
}


glm::vec3 light::Final_light(glm::vec3 K_A, glm::vec3 K_D,glm::vec3 K_S, int user_angle) {
	glm::vec3 final_color ;
	Find_I_S(K_S, user_angle);
	Find_I_A(K_A);
	Find_I_D(K_D);
	final_color =this->I_S + this->I_D + this->I_A;
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

