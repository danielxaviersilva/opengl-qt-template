#ifndef CILINDER_H
#define CILINDER_H

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


class Cilinder
{

   int m_thetaRes;
   int m_heightRes;
    float m_height;
    float m_radius;
    glm::vec3 m_center;
    bool m_initialized;

    typedef struct _cilinder_attributes{
        glm::vec4 vertex;
        glm::vec4 normal;
//        glm::vec2 textureCoords;
        _cilinder_attributes(glm::vec4 inVertex, glm::vec4 inNormal): vertex(inVertex), normal(inNormal){};

    } cilinderAttributes;

    std::vector <cilinderAttributes> m_cilinderAttributes;
    GLuint surface_vbo;
    GLuint normals_vbo;

    VertexBuffer m_vbo;
    VertexArray m_vao;
    shader m_program;

    float m_verticesSize;

    void setCilinderSurface();
    void setLighting();

public:
    Cilinder(int thetaRes = 30, int heightRes = 30, float R = 0.25f, float height = 0.5f, glm::vec3 center = glm::vec3(0.0));
    void initialize();
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void render();
    void _check_gl_error(const char *file, int line);
};

#endif // CILINDER_H
