#ifndef CILINDER_H
#define CILINDER_H

#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>
#include <glm/gtx/transform.hpp>
#include "../Utilities/vertexarray.h"
#include "../Utilities/vertexbuffer.h"
#include "../Utilities/shader.h"
#include "../Utilities/cgutilities.h"


class Cilinder : protected CGUtilities
{

   int m_thetaRes;
   int m_heightRes;
    float m_height;
    float m_radius;
    glm::vec3 m_center;
    glm::vec3 m_mainAxis;
    bool m_initialized;

    typedef struct _cilinder_attributes{
        glm::vec4 vertex;
        glm::vec4 normal;
//        glm::vec2 textureCoords;
        _cilinder_attributes(glm::vec4 inVertex, glm::vec4 inNormal): vertex(inVertex), normal(inNormal){};

    } cilinderAttributes;

    std::vector <cilinderAttributes> m_cilinderAttributes;

    VertexBuffer m_vbo;
    VertexArray m_vao;
    shader m_program;

    float m_verticesSize;

    void setCilinderSurface();
    void setLighting();

public:

    Cilinder(int thetaRes = 30, int heightRes = 30, float R = 0.25f, float height = 0.5f,
             glm::vec3 center = glm::vec3(0.0), glm::vec3 mainAxis = glm::vec3(0.0, 1.0, 0.0));
    void initialize();
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void render();
};

#endif // CILINDER_H
