#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <QOpenGLFunctions>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    float m_projectionParameters[6];

    bool isOrtho;
    float m_cameraSpeed;
    float m_mvpMatrixLoc;


public:
    Camera(unsigned int program = 0,
            std::string mvpMatrixName = "",
           glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f));

    void setMvpMatrixLoc(unsigned int program, std::string mvpMatrixName);
    void updateMvpMatrix(unsigned int program);


    void walkAround(glm::vec3 direction);
    float cameraSpeed() const;
    void setCameraSpeed(float cameraSpeed);

    void scaleWorld(float scaleFactor);
};
#endif // CAMERA_H
