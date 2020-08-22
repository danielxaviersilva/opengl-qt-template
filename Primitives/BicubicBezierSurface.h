#ifndef BICUBICBEZIERSURFACE_H
#define BICUBICBEZIERSURFACE_H


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

class BicubicBezierSurface : CGUtilities
{
    int m_resolution;

    VertexBuffer m_vbo;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;
    Shader m_program;

    bool m_initialized;
    int m_verticesSize;

public:
    BicubicBezierSurface();

    void setBicubicBezierSurface(std::vector<glm::vec3> controlPoints, int res = 100);


    void initialize();
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void setColor(glm::vec4 color);
    void render();
};

#endif // BICUBICBEZIERSURFACE_H
