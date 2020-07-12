#include "SphereSet.h"

SphereSet::SphereSet(int theta, int phi):
    m_thetaRes(theta),m_phiRes(phi), m_initialized(false), m_attributesFlag(false)
{
    m_materialAmbientColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    m_materialDiffuseColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    m_materialSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_shineness             = 5.0f;
}

void SphereSet::addSphere(float r, glm::vec3 center, glm::vec3 axis)
{
    if(!m_initialized) {
        std::cerr << "SphereSet.addSphere(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }

    if(r < 0.0f){
        std::cerr << "SphereSet::addSphere(): for God's sake, dont send to me a negative radius (" << r << ")" << std::endl;
        return;
    }
    m_attributesFlag = true;
    m_sphereAttributes.push_back({r, center, shiftYtoAxisMatrix(axis)});
    m_SphereAttributesVBO.updateBufferData(m_sphereAttributes.data(), m_sphereAttributes.size()*sizeof(sphereAttributes));
    _check_gl_error(__FILE__,__LINE__);
}

void SphereSet::initialize(LightSource* lightSource)
{
    if(!m_initialized)
    {
        m_initialized = true;
        m_program.loadProgram("./renderInstances.vert","./renderInstances.frag");
        m_program.useProgram();

        m_idxBuffer.genBuffer();
        m_SphereVBO.genBuffer();
        m_SphereAttributesVBO.genBuffer();

        setVaoLayout();
        setSphereSurface();


        setProjectionMatrix(glm::mat4(1.0f));
        setMVMatrix(glm::mat4(1.0f));

        m_lightSource = lightSource;
        m_lightSource->updateLightSource(&m_program);
        updateMaterialLighting();

        m_vao.unbind();
        m_program.release();
        return;
    }
    std::cerr << "SphereSet::initialize(): Class already initialized" << std::endl;

}

void SphereSet::render()
{
    if (!m_initialized){
        std::cerr << "SphereSet.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    if (!m_attributesFlag){
        std::cerr << "SphereSet.rendering(): not rendering because there isn't any sphere to render. Use method add(float radius, glm::vec3 center)." << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();

    glEnable(GL_CULL_FACE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElementsInstanced(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr, int(m_sphereAttributes.size()));
    _check_gl_error(__FILE__,__LINE__);



    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    m_idxBuffer.unbind();
    m_vao.unbind();
    m_program.release();
}

void SphereSet::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void SphereSet::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void SphereSet::setLightSource(LightSource *lightSource)
{
    m_lightSource = lightSource;
    m_lightSource->updateLightSource(&m_program);
}

void SphereSet::setMaterialLighting(glm::vec4 materialAmbientColor, glm::vec4 materialDiffuseColor, glm::vec4 materialSpecularColor, float shineness)
{
    m_materialAmbientColor  = materialAmbientColor;
    m_materialDiffuseColor  = materialDiffuseColor;
    m_materialSpecularColor = materialSpecularColor;
    m_shineness             = shineness;
    updateMaterialLighting();
}


void SphereSet::updateMaterialLighting()
{
    GLshort materialAmbientColorLoc  =  m_program.getUniformLocation( "u_materialLighting.ambient");
    GLshort materialDiffuseColorLoc  =  m_program.getUniformLocation( "u_materialLighting.diffuse");
    GLshort materialSpecularColorLoc =  m_program.getUniformLocation( "u_materialLighting.specular");
    GLshort shinenessLoc             =  m_program.getUniformLocation( "u_materialLighting.shineness");

    m_program.useProgram();
    glUniform3fv(materialAmbientColorLoc ,1,glm::value_ptr(m_materialAmbientColor  ));
    glUniform3fv(materialDiffuseColorLoc ,1,glm::value_ptr(m_materialDiffuseColor  ));
    glUniform3fv(materialSpecularColorLoc,1,glm::value_ptr(m_materialSpecularColor ));
    glUniform1f (shinenessLoc              ,m_shineness                          );
}

void SphereSet::setSphereSurface()
{
    std::vector <baseSphereAttributes> sphereAttributesBuffer;

    std::vector<unsigned int> idxSet;
    idxSet.reserve(6*m_thetaRes*(m_phiRes-1));
    sphereAttributesBuffer.reserve(m_phiRes*m_thetaRes);
    //Interval of meshes:
    //phi: [-pi/2, pi/2]
    //theta: [0, 2pi)
    //The loop is designed to make a closed circle on theta,
    //The only repetition of elements on buffer are when phi = ±pi/2
    for (int i = 0; i < m_phiRes; i++)
    {
        float m_phi =  M_PI/2 - M_PI*i/(m_phiRes - 1);
        for(int j = 0; j<m_thetaRes; j++)
        {
             float m_theta = 2*(M_PI)*j/(m_thetaRes);

             sphereAttributesBuffer.emplace_back(baseSphereAttributes(
             glm::vec4(cos(m_phi)*cos(m_theta), sin(m_phi), cos(m_phi)*sin(m_theta), 1.0f), //vertex
             glm::vec4(cos(m_phi)*cos(m_theta), sin(m_phi), cos(m_phi)*sin(m_theta), 0.0f))); //normals
              if (i < m_phiRes -1)
              {
                    idxSet.emplace_back(i*m_thetaRes + j                   );
                    idxSet.emplace_back((i+1)*m_thetaRes + j               );
                    idxSet.emplace_back((i+1)*m_thetaRes + (j+1)%m_thetaRes);
                    idxSet.emplace_back(i*m_thetaRes + j                   );
                    idxSet.emplace_back((i+1)*m_thetaRes + (j+1)%m_thetaRes);
                    idxSet.emplace_back(i*m_thetaRes + (j+1)%m_thetaRes    );
              }
        }
     }
     m_verticesSize = idxSet.size();
     m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(baseSphereAttributes));
     _check_gl_error(__FILE__,__LINE__);


        m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
        m_idxBuffer.unbind();
        _check_gl_error(__FILE__,__LINE__);
}



int SphereSet::getThetaRes() const
{
    return m_thetaRes;
}

int SphereSet::getPhiRes() const
{
    return m_phiRes;
}

void SphereSet::setVaoLayout()
{
    m_vao.bind();
    int locVertex = m_program.getAttribLocation("vertex");
    int locNormal = m_program.getAttribLocation("normal");

    int locDisplacement = m_program.getAttribLocation("instance_displacement");
    int locRadius =  m_program.getAttribLocation("instance_radius");
    int locReorientMatrix =  m_program.getAttribLocation("instance_reorientationMatrix");

    m_vao.push<float>(locVertex, 4);
    m_vao.push<float>(locNormal, 4);
    m_vao.addBuffer(m_SphereVBO);
    m_vao.clearLayout(); //Clearing to support other buffer, that has the Center/Radius parameter.

    m_vao.push<float>(locRadius,1, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locDisplacement,     3, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix,   4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix+1, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix+2, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix+3, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_SphereAttributesVBO.updateBufferData(m_sphereAttributes.data(), m_sphereAttributes.size()*sizeof(sphereAttributes));
    m_vao.addBuffer(m_SphereAttributesVBO);
    m_vao.clearLayout();
    m_vao.unbind();
    _check_gl_error(__FILE__,__LINE__);

}
//void SphereSet::setTextureCoords()
//{
//    float direction[3];
//    for (int i = 0; i < spherePoints; i++)
//    {
//        direction[0] = surface[4*i]   - center[0];
//        direction[1] = surface[4*i+1] - center[1];
//        direction[2] = surface[4*i+2] - center[2];

//        textureCoords.push_back((atan2(direction[1],direction[0])/(2*M_PI) + 0.5)*(float(spherePoints))/(float(spherePoints) + 1));
//        textureCoords.push_back((0.5-asin(direction[2])/M_PI)  *  (float(spherePoints))/(float(spherePoints) + 1));
//    }

//}
