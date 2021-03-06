#ifndef QBALLRENDERER_H
#define QBALLRENDERER_H
#include "QBall.h"

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
#include "../LightSource.h"
#include "../Utilities/Texture.h"


class QBallRenderer: protected CGUtilities
{

protected:
    VertexBuffer m_SphereVBO;
    VertexBuffer m_SphereAttributesVBO;
//    VertexBuffer m_SphereODFMapVBO;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;
    Shader m_program;

//    LightSource * m_lightSource;

    int m_verticesSize;


//    glm::vec4 m_materialAmbientColor, m_materialDiffuseColor, m_materialSpecularColor;
    float     m_shineness;

    int m_ODFsize;

    unsigned int m_InstancesCount;

    bool m_initialized;
    bool m_attributesFlag;

    int m_slot;

    QBall * m_qBallRef;
    Texture m_ODFMapTexture;

public:
    QBallRenderer();
    virtual ~QBallRenderer();
    void addGlyph(const std::vector<float> &ODFList, glm::vec3 center, glm::vec3 axis = glm::vec3(0.0f,1.0f,0.0f), float scale = 1.0f);
    void addThreeAngleGlyph(const std::vector<float> &ODFList, float scale = 1.0f);
//    void addSphere(float r = 0.5, glm::vec3 center = glm::vec3(0.0f));


    virtual void initialize(QBall* qBall);
    virtual void render(const std::vector<unsigned int>& odfIndexSet);




    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);

    void instantDrawGlyphs();

protected:
    void setVaoLayout();
    void setSphereSurface();
//    void updateODFMapTexture();

    virtual void setInstancedVertexAttribBuffer(const std::vector<unsigned int>& odfIndexSet);

};


#endif // QBALLRENDERER_H
