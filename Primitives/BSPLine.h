#ifndef BSPLINE_H
#define BSPLINE_H

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


class BSPLine : protected CGUtilities
{
    int m_resolution;

    VertexBuffer m_vbo;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;
    Shader m_program;

    bool m_initialized;
    int m_verticesSize;

    std::vector<float> setBSPLineDomain(const std::vector<float>& controlPointSet, const std::vector<float> &nodes, const int &degree, const int & resolution);
public:
    BSPLine();

    void initialize();
    void setBSPLine(std::vector<glm::vec2> controlPointSet, std::vector<float> nodes, int degree, float height, int resolution);
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void setColor(glm::vec4 color);
    void render();

};

#endif // BSPLINE_H
