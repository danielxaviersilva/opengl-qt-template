#ifndef ICOSPHERE_H
#define ICOSPHERE_H


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

//describes sphere that each coordinate is in vec4 form (x,y,z alfa=1).



class IcoSphere : public CGUtilities
{


    typedef struct _base_sphere_attributes{
        glm::vec4 vertex;
        glm::vec4 normal;
//        glm::vec2 textureCoords;
        _base_sphere_attributes(glm::vec4 inVertex, glm::vec4 inNormal): vertex(inVertex), normal(inNormal){};

    } baseSphereAttributes;

    typedef struct _sphere_attributes{
        float radius;
        glm::vec3 center;
        glm::mat4 reorientMatrix;

    } sphereAttributes;

    std::vector<sphereAttributes> m_sphereAttributes;

    VertexBuffer m_SphereVBO;
    VertexBuffer m_SphereAttributesVBO;
    VertexArray m_vao;
    IndexBuffer m_idxBuffer;

    Shader m_program;
    int m_verticesSize;

    LightSource * m_lightSource;

    glm::vec4 m_materialAmbientColor, m_materialDiffuseColor, m_materialSpecularColor;
    float     m_shineness;

    int m_vertexAmountThreshold;


    bool m_initialized;
    bool m_attributesFlag;



public:
    IcoSphere(int vertexAmount = 1000);
    void addSphere(float r = 0.5, glm::vec3 center = glm::vec3(0.0f), glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f));
    void initialize(LightSource* lightSource = new LightSource);

    void setSphereSurface();



    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMVMatrix(glm::mat4 mvMatrix);
    void updateMaterialLighting();
    void render();
    void setMaterialLighting(glm::vec4 materialAmbientColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                             glm::vec4 materialDiffuseColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
                             glm::vec4 materialSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                             float     shineness             = 5.0f                              );

    void setLightSource(LightSource *lightSource);


    int getThetaRes() const;
    int getPhiRes() const;

    void shrinkVecBaseAttributes(std::vector<baseSphereAttributes> &V, std::vector<unsigned int> &index, bool verbose);
protected:
    void shrinkVecBaseAttributes(std::vector<glm::vec4> &V, std::vector<int> &index, bool verbose);
    void shrinkVecBaseAttributes(std::vector<baseSphereAttributes> &V, std::vector<int> &index, bool verbose);
private:
    void setVaoLayout();
    std::vector<float> computeIcosahedronVertices();
    std::vector<baseSphereAttributes> vertexListToVertexNormalAttributes(const std::vector<glm::vec3> vertex);
public:
};

#endif // ICOSPHERE_H
