#pragma once
#include <MeshModel.h>
#include <glm/glm.hpp>
#include <glm\ext\matrix_transform.hpp>

class light : public MeshModel  
{
private:

    glm::vec3 I;
    glm::vec3 N;
    glm::vec3 V;
    glm::vec3 R;
    
    glm::vec3 I_A;
    glm::vec3 I_S;
    glm::vec3 I_D;
    glm::mat4x4 localTransform;
    glm::vec3 position;


public:
    light(glm::vec3 I, glm::vec3 N, glm::vec3 V, glm::vec3 L_A, glm::vec3 L_D, glm::vec3 L_S,
        std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
    glm::vec3 light::Mul(float x, glm::vec3 vec);
    glm::vec3 L_A;
    glm::vec3 L_S;
    glm::vec3 L_D;
    glm::vec3 paralel;
    void SetTransformX(float val);
    void SetTransformY(float val);
    void SetTransformZ(float val);
    glm::mat4 GetTransform();
    glm::vec3 GetPosVec();
    float GetCosAlpha(glm::vec3 v1, glm::vec3 v2);
    void SetPos(glm::vec3 new_pos);
    float user_angle;


    
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
    glm::vec3 light::Final_light(glm::vec3 K_A, glm::vec3 K_D, glm::vec3 K_S, int user_angle, glm::vec3 V);

};