#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>
#include "Utilities/VertexArray.h"
#include "Utilities/VertexBuffer.h"
#include "Utilities/Shader.h"




class LightSource
{
public:
    enum ShadingMode{
        PHONG,
        GOURAUD,
        FLAT,

    };
private:

    ShadingMode m_shadingModel;
    glm::vec4 m_position;
    glm::vec3 m_spotDirection;
    float m_spotExponent;
    float m_spotCutoff;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    glm::vec4 m_specularExpectatorPosition;

    std::string m_shadingModelName;
    std::string m_lightPositionName;
    std::string m_spotDirectionName;
    std::string m_spotExponentName;
    std::string m_spotCutoffName;
    std::string m_lightAmbientColorName;
    std::string m_lightDiffuseColorName;
    std::string m_lightSpecularColorName;
    std::string m_specularExpectatorPositionName;



    Shader* m_program;

    GLshort m_shadingModelLoc;
    GLshort m_lightPositionLoc;
    GLshort m_spotDirectionLoc;
    GLshort m_spotExponentLoc;
    GLshort m_spotCutoffLoc;
    GLshort m_lightAmbientColorLoc;
    GLshort m_lightDiffuseColorLoc;
    GLshort m_lightSpecularColorLoc;
    GLshort m_specularExpectatorPositionLoc;

public:
    LightSource(std::string shadingModelName       = "u_shadingModel",
                std::string lightPositionName      = "u_lightSource.position",
                std::string spotDirectionName      = "u_lightSource.spotDirection",
                std::string spotExponentName       = "u_lightSource.spotExponent",
                std::string spotCutoffName         = "u_lightSource.spotCutoff",
                std::string lightAmbientColorName  = "u_lightSource.ambient",
                std::string lightDiffuseColorName  = "u_lightSource.diffuse",
                std::string lightSpecularColorName = "u_lightSource.specular",
                std::string specularExpectatorPositionLoc = "u_expectatorPosition");

    void setToDefault();
    void updateLightSource();

    void updateLightSource(Shader *program);

    void setShadingModel(const ShadingMode &shadingModel);
    void setPosition(const glm::vec4 &position);
    void setSpotDirection(const glm::vec3 &spotDirection);
    void setSpotExponent(float spotExponent);
    void setSpotCutoff(float spotCutoff);
    void setAmbient(const glm::vec4 &ambient);
    void setDiffuse(const glm::vec4 &diffuse);
    void setSpecular(const glm::vec4 &specular);
    void setSpecularExpectatorPosition(const glm::vec4 &specularExpectatorPosition);

    void setShadingModel(const ShadingMode &shadingModel, Shader* program);
    void setPosition(const glm::vec4 &position, Shader* program);
    void setSpotDirection(const glm::vec3 &spotDirection, Shader* program);
    void setSpotExponent(float spotExponent, Shader* program);
    void setSpotCutoff(float spotCutoff, Shader* program);
    void setAmbient(const glm::vec4 &ambient, Shader* program);
    void setDiffuse(const glm::vec4 &diffuse, Shader* program);
    void setSpecular(const glm::vec4 &specular, Shader* program);
    void setSpecularExpectatorPosition(const glm::vec4 &specularExpectatorPosition, Shader* program);
};

#endif // LIGHTSOURCE_H
