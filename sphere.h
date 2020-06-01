#ifndef SPHERE_H
#define SPHERE_H

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

//describes sphere that each coordinate is in vec4 form (x,y,z alfa=1).


class Sphere
{
    int m_thetaRes;
    int m_phiRes;
    int m_sphereSize;
    int m_spherePoints;
    float m_radius;
    glm::vec3 m_center;

    bool m_initialized;

    typedef struct _sphere_attributes{
        glm::vec4 vertex;
        glm::vec4 normal;
//        glm::vec2 textureCoords;
        _sphere_attributes(glm::vec4 inVertex, glm::vec4 inNormal): vertex(inVertex), normal(inNormal){};

    } sphereAttributes;

    std::vector <sphereAttributes> m_sphereAttributes;
    GLuint surface_vbo;
    GLuint normals_vbo;

    VertexBuffer m_vbo;
    VertexArray m_vao;
    shader m_program;

    float m_verticesSize;

public:
   // sphere();
//    Sphere(int theta = 30, int phi = 30, float r = 0.5, float xc = 0, float yc = 0, float zc = 0);
    Sphere(int theta = 30, int phi = 30, float r = 0.5, glm::vec3 center = glm::vec3(0.0f));

    void initialize();

    void setSphereSurface();
    void setSphereNormal();


//    void setTextureCoords();

    int getThetaRes();
    int getPhiRes();
    int getSphereSize();
    int getSpherePoints();


    float getRadius() const;

//    void uploadSphere();

    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void setLighting();
    void render();


private:
    void _check_gl_error(const char *file, int line);
};

#endif // SPHERE_H
