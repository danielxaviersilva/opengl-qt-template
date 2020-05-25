#include "camera.h"
#include <iostream>

float Camera::cameraSpeed() const
{
    return m_cameraSpeed;
}

void Camera::setCameraSpeed(float cameraSpeed)
{
    m_cameraSpeed = cameraSpeed;
}

void Camera::scaleWorld(float scaleFactor)
{
    m_projectionParameters[0] *=scaleFactor;
    m_projectionParameters[1] *=scaleFactor;
    m_projectionParameters[2] *=scaleFactor;
    m_projectionParameters[3] *=scaleFactor;
    m_projectionParameters[4] *=scaleFactor;
    m_projectionParameters[5] *=scaleFactor;

    m_projection = glm::ortho( m_projectionParameters[0],
                               m_projectionParameters[1],
                               m_projectionParameters[2],
                               m_projectionParameters[3],
                               m_projectionParameters[4],
                               m_projectionParameters[5]);

}

Camera::Camera(unsigned int program,
               std::string mvpMatrixName,
               glm::vec3 cameraPos,
               glm::vec3 cameraFront,
               glm::vec3 cameraUp):
    m_cameraPos(cameraPos),
    m_cameraFront(cameraFront),
    m_cameraUp(cameraUp)
{
    m_view = glm::lookAt(m_cameraPos, m_cameraPos, m_cameraUp);
    m_projectionParameters[0] = m_projectionParameters[2] = m_projectionParameters[4] = -1;
    m_projectionParameters[1] = m_projectionParameters[3] = m_projectionParameters[5] = 1;

    m_projection = glm::ortho( m_projectionParameters[0],
                               m_projectionParameters[1],
                               m_projectionParameters[2],
                               m_projectionParameters[3],
                               m_projectionParameters[4],
                               m_projectionParameters[5]);
    m_cameraSpeed = 0.1f;

    m_mvpMatrixLoc = -1;
    if (mvpMatrixName != "" || program != 0)
    {
        m_mvpMatrixLoc = glGetUniformLocation(program, mvpMatrixName.c_str());
        if (m_mvpMatrixLoc == -1)
            std::cerr << "MVP Matrix not found in the program" << std::endl;
    }

}

void Camera::setMvpMatrixLoc(unsigned int program, std::string mvpMatrixName)
{
    m_mvpMatrixLoc = glGetUniformLocation(program, mvpMatrixName.c_str());
    if (m_mvpMatrixLoc == -1)
        std::cerr << "MVP Matrix not found in the program" << std::endl;

}

void Camera::updateMvpMatrix(unsigned int program)
{
    if (m_mvpMatrixLoc == -1)
    {
        std::cerr << "Not possible to update mvpMatrix. The localization of it in the current program wasn't found" << std::endl;
        return;
    }
    glm::mat4 mvp = m_projection*m_view;
    glUniformMatrix4fv(program, 1, GL_FALSE, glm::value_ptr(mvp));
}

void Camera::walkAround(glm::vec3 direction)
{
    m_cameraPos += m_cameraSpeed*direction;
    m_view = glm::lookAt(m_cameraPos, m_cameraPos, m_cameraUp);
}


