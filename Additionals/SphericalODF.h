#ifndef SPHERICALODF_H
#define SPHERICALODF_H


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

//describes sphere that each coordinate is in vec4 form (x,y,z alfa=1).

class SphereODF : protected CGUtilities
{


    typedef struct _base_sphere_attributes{
        glm::vec4 vertex;
        glm::vec4 normal;
//        glm::vec2 textureCoords;
        _base_sphere_attributes(glm::vec4 inVertex, glm::vec4 inNormal): vertex(inVertex), normal(inNormal){};

    } baseSphereAttributes;

    typedef struct _instanced_sphere_attributes{
        glm::vec3 center;
        glm::mat4 reorientMatrix;
    } InstancedSphereAttributes;

    std::vector<InstancedSphereAttributes> m_instancedSphereAttributes;

    std::vector<float> m_ODFMap;

    VertexBuffer m_SphereVBO;
    VertexBuffer m_SphereAttributesVBO;
//    VertexBuffer m_SphereODFMapVBO;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;
    Shader m_program;

    LightSource * m_lightSource;

    int m_verticesSize;


    glm::vec4 m_materialAmbientColor, m_materialDiffuseColor, m_materialSpecularColor;
    float     m_shineness;

    int m_thetaRes;
    int m_phiRes;

    int m_InstancesCount;

    bool m_initialized;
    bool m_attributesFlag;

    int m_slot;

    Texture m_ODFMapTexture;


public:
    SphereODF(int theta = 30, int phi = 30);
    void addGlyph(const std::vector<float> &ODFList, glm::vec3 center, glm::vec3 axis = glm::vec3(0.0f,1.0f,0.0f), float scale = 1.0f);
    void addThreeAngleGlyph(const std::vector<float> &ODFList, float scale = 1.0f);
//    void addSphere(float r = 0.5, glm::vec3 center = glm::vec3(0.0f));


    void initialize(LightSource * lightSource = new LightSource);
    void render();


    void setMaterialLighting(glm::vec4 materialAmbientColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                             glm::vec4 materialDiffuseColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
                             glm::vec4 materialSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                             float     shineness             = 5.0f                              );

    void updateMaterialLighting();


    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);

    void setLightSource(LightSource *lightSource);


    int getThetaRes() const;
    int getPhiRes() const;

private:
    void setVaoLayout();
    void setSphereSurface();

};

#endif // SPHERICALODF_H
