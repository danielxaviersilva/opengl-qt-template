#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>
#include "../Utilities/VertexArray.h"
#include "../Utilities/VertexBuffer.h"
#include "../Utilities/IndexBuffer.h"
#include "../Utilities/Shader.h"
#include "../Utilities/CGUtilities.h"

//describes sphere that each coordinate is in vec4 form (x,y,z alfa=1).


class Sphere : protected CGUtilities
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

    VertexBuffer m_vbo;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;
    Shader m_program;

    float m_verticesSize;

public:
    Sphere(int theta = 30, int phi = 30, float r = 0.5, glm::vec3 center = glm::vec3(0.0f));

    void initialize();

    void setSphereSurface();

    int getThetaRes();
    int getPhiRes();
    int getSphereSize();
    int getSpherePoints();


    float getRadius() const;

    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void setLighting();
    void render();
};

#endif // SPHERE_H
