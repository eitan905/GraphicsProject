#pragma once
#include <glm/glm.hpp>
#include <glm\ext\matrix_transform.hpp>

class light  {
private:

    glm::vec3 I;
    glm::vec3 N;
    glm::vec3 V;
    glm::vec3 R;
    glm::vec3 L_A;
    glm::vec3 L_S;
    glm::vec3 L_D;
    glm::vec3 I_A;
    glm::vec3 I_S;
    glm::vec3 I_D;

public:
    glm::vec3 light::Mul(int x, glm::vec3 vec);
    glm::vec4 color_L_A;
    glm::vec4 color_L_D;
    glm::vec4 color_L_S;

    

    light::light(glm::vec3 I, glm::vec3 N, glm::vec3 V, glm::vec3 L_A, glm::vec3 L_D, glm::vec3 L_S);
    void light::Set_I(glm::vec3 I);
    void light::Set_N(glm::vec3 N);
    void light::Set_V(glm::vec3 V);
    void light::Set_R(glm::vec3 R);
    void light::Set_L_A(glm::vec3 L_A);
    void light::Set_L_D(glm::vec3 L_D);
    void light::Set_L_S(glm::vec3 L_S);

    void light::Find_I_A(glm::vec3 K_A); 
    void light::Find_I_S(glm::vec3 K_S, int user_angle);
    void light::Find_I_D(glm::vec3 K_D);
    glm::vec3 light::Final_light(glm::vec3 K_A, glm::vec3 K_D, glm::vec3 K_S, int user_angle);

};