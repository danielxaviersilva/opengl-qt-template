#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>

#include "../Utilities/IndexBuffer.h"
#include "../Utilities/VertexArray.h"
#include "../Utilities/VertexBuffer.h"
#include "../Utilities/Shader.h"
#include "../Utilities/CGUtilities.h"



class Cube : protected CGUtilities
{



    int m_cubeCount;

    float     m_edge;
    glm::vec3 m_center;
    glm::vec3 m_mainAxis;

    bool m_initialized;

    typedef struct _cube_attributes{
        glm::vec4 vertex;
        glm::vec4 normal;
//        glm::vec2 textureCoords;
        _cube_attributes(glm::vec4 inVertex, glm::vec4 inNormal): vertex(inVertex), normal(inNormal){};

    } cubeAttributes;

    std::vector <cubeAttributes> m_cubeAttributes;

    GLuint surface_vbo;
    GLuint normals_vbo;

    VertexBuffer m_vbo;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;
    Shader m_program;

    float m_verticesSize;

public:
    Cube(float edge = 0.25, glm::vec3 center = glm::vec3(0.0f), glm::vec3 rotationAxis = glm::vec3(0.0f,1.0f,0.0f));

    void initialize();

    void setCubeSurface();
    void setLighting();
//    void setTextureCoords();


    float getRadius() const;

    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void render();

private:
    void _check_gl_error(const char *file, int line);



};

#endif // CUBE_H
