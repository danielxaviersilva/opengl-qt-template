#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "shader.h"

enum ShadingMode{
    FLAT,
    SMOOTH,
    PHONG
};


class LightSource
{
    glm::vec4 m_position;
    glm::vec3 m_spotDirection;
    float m_spotExponent;
    float m_spotCutoff;

    glm::vec4 m_ambientColor;
    glm::vec4 m_diffuseColor;
    glm::vec4 m_specularColor;
    ShadingMode m_shading;


public:
    LightSource();

};

#endif // LIGHTSOURCE_H
