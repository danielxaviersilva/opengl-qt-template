#include "SphericalODF.h"

SphereODF::SphereODF(int theta, int phi):
    m_thetaRes(theta),m_phiRes(phi), m_initialized(false), m_attributesFlag(false), m_slot(0)
{
    m_materialAmbientColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    m_materialDiffuseColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    m_materialSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_shineness             = 5.0f;
}

void SphereODF::addGlyph(const std::vector<float> &ODFList, glm::vec3 center, glm::vec3 axis, float scale)
{
    if(!m_initialized) {
        std::cerr << "SphereODF.addSphere(): Class not initialized yet. Use method initialize()" << std::endl;
        return;
    }

    std::cout << "Size ODF: "<< ODFList.size() << " Size do domínio da esfera: " << m_meshPointsAmount << std::endl;
    int currentIdx = 0;

    for (int i = 0; i < m_meshPointsAmount; i++)
        m_ODFMap.push_back((scale*ODFList[currentIdx++]));



    m_instancedSphereAttributes.push_back({center, shiftYtoAxisMatrix(axis)});
    if (!m_attributesFlag)
        m_attributesFlag = true;

    m_InstancesCount = int(m_instancedSphereAttributes.size());
     m_SphereAttributesVBO.updateBufferData(m_instancedSphereAttributes.data(), m_InstancesCount*sizeof(InstancedSphereAttributes));

    _check_gl_error(__FILE__,__LINE__);


    m_program.useProgram();
    m_ODFMapTexture.uploadTexture(m_ODFMap, m_meshPointsAmount, m_InstancesCount);
    int instanceCountLoc = m_program.getUniformLocation("u_instanceCount");
    glUniform1f(instanceCountLoc , float(m_InstancesCount));
    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);
    m_ODFMapTexture.Unbind();

}

void SphereODF::addThreeAngleGlyph(const std::vector<float> &ODFList, float scale)
{
    if(!m_initialized) {
        std::cerr << "SphereODF.addSphere(): Class not initialized yet. Use method initialize()" << std::endl;
        return;
    }

    std::cout << "Size ODF: "<< ODFList.size() << " Size do domínio da esfera: " << m_meshPointsAmount << std::endl;
    int currentIdx;
    currentIdx = 0;
    for (int i = 0; i < m_meshPointsAmount; i++)
        m_ODFMap.push_back((scale*ODFList[currentIdx++]));
    currentIdx = 0;
    for (int i = 0; i < m_meshPointsAmount; i++)
        m_ODFMap.push_back((scale*ODFList[currentIdx++]));
    currentIdx = 0;
    for (int i = 0; i < m_meshPointsAmount; i++)
        m_ODFMap.push_back((scale*ODFList[currentIdx++]));



    m_instancedSphereAttributes.push_back({glm::vec3( 0.5f,  0.5, 0.0f), shiftYtoAxisMatrix(glm::vec3(0.0f,0.0f,1.0f))});
    m_instancedSphereAttributes.push_back({glm::vec3( -0.5f,  -0.5, 0.0f), shiftYtoAxisMatrix(glm::vec3(0.0f,1.0f,0.0f))});
    m_instancedSphereAttributes.push_back({glm::vec3( 0.5f,  -0.5, 0.0f), shiftYtoAxisMatrix(glm::vec3(1.0f,0.0f,0.0f))});


    if (!m_attributesFlag)
        m_attributesFlag = true;

    m_InstancesCount = int(m_instancedSphereAttributes.size());
     m_SphereAttributesVBO.updateBufferData(m_instancedSphereAttributes.data(), m_InstancesCount*sizeof(InstancedSphereAttributes));

    _check_gl_error(__FILE__,__LINE__);


//    m_program.useProgram();
//    m_ODFMapTexture.uploadTexture(m_ODFMap, m_thetaRes*m_phiRes, m_InstancesCount);
//    int instanceCountLoc = m_program.getUniformLocation("u_instanceCount");
//    glUniform1f(instanceCountLoc , float(m_InstancesCount));
//    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
//    m_ODFMapTexture.Bind(m_slot);


}

void SphereODF::initialize(LightSource *lightSource)
{
    if(!m_initialized)
    {

        m_initialized = true;
        m_program.loadProgram("./renderQBallGlyphs.vert","./renderQBallGlyphs.frag");
        m_program.useProgram();

        m_SphereAttributesVBO.genBuffer();
        m_SphereVBO.genBuffer();
        m_idxBuffer.genBuffer();

        setVaoLayout();
        setSphereSurface(); //this method assign the m_SphereVBO buffer with a unitary sphere

        int glyphResolutionloc = m_program.getUniformLocation("u_glyphResolution");
        glUniform1f(glyphResolutionloc,m_meshPointsAmount);

        setProjectionMatrix(glm::mat4(1.0f));
        setMVMatrix(glm::mat4(1.0f));

        m_lightSource = lightSource;
        m_lightSource->updateLightSource(&m_program);
        updateMaterialLighting();
        m_program.release();
    }

}

void SphereODF::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void SphereODF::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void SphereODF::setLightSource(LightSource *lightSource)
{
    m_lightSource = lightSource;
    m_lightSource->updateLightSource(&m_program);
}

void SphereODF::setSphereSurface()
{
//    std::vector <baseSphereAttributes> sphereAttributesBuffer;
    std::vector <glm::vec3> sphereAttributesBuffer;

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
        float phi =  M_PI/2 - M_PI*i/(m_phiRes - 1);
        for(int j = 0; j<m_thetaRes; j++)
        {
             float theta = 2*(M_PI)*j/(m_thetaRes);

//             sphereAttributesBuffer.emplace_back(baseSphereAttributes(
//             glm::vec4(cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta), 1.0f), //vertex
//             glm::vec4(cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta), 0.0f))); //normals

             sphereAttributesBuffer.emplace_back(glm::vec3
             (cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta))); //normals

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

        shrinkVec3(sphereAttributesBuffer, idxSet, true);
        m_meshPointsAmount = sphereAttributesBuffer.size();

        m_verticesSize = idxSet.size();//sphereAttributesBuffer.size();
//        m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(baseSphereAttributes));
          m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(glm::vec3));
        _check_gl_error(__FILE__,__LINE__);


        m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
        m_idxBuffer.unbind();
        _check_gl_error(__FILE__,__LINE__);
}

void SphereODF::updateODFMapTexture()
{
    m_ODFMapTexture.uploadTexture(m_ODFMap, m_meshPointsAmount, m_InstancesCount);
    int instanceCountLoc = m_program.getUniformLocation("u_instanceCount");
    glUniform1f(instanceCountLoc , float(m_InstancesCount));
    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);
    m_ODFMapTexture.Unbind();
}

void SphereODF::setMaterialLighting(glm::vec4 materialAmbientColor, glm::vec4 materialDiffuseColor, glm::vec4 materialSpecularColor, float shineness)
{
    m_materialAmbientColor  = materialAmbientColor;
    m_materialDiffuseColor  = materialDiffuseColor;
    m_materialSpecularColor = materialSpecularColor;
    m_shineness             = shineness;

    updateMaterialLighting();
}

void SphereODF::updateMaterialLighting()
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

void SphereODF::render()
{
    if (!m_initialized)
    {
        std::cerr << "SphereODF.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    if (!m_attributesFlag){
        std::cerr << "SphereODF.rendering(): not rendering because there isn't any sphere to render. Use method add(float radius, glm::vec3 center)." << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();
    updateODFMapTexture();

//    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);

//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

#ifndef QT_NO_DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    glDrawElementsInstanced(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr, int(m_instancedSphereAttributes.size()));
    _check_gl_error(__FILE__,__LINE__);

//    glDisable(GL_DEPTH_TEST);
#ifndef QT_NO_DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    glDisable(GL_CULL_FACE);

    m_ODFMapTexture.deleteTexture();
    m_idxBuffer.unbind();
    m_vao.unbind();
    m_program.release();
}

int SphereODF::getThetaRes() const
{
    return m_thetaRes;
}

int SphereODF::getPhiRes() const
{
    return m_phiRes;
}

void SphereODF::instantDrawGlyphs()
{
    if (!m_initialized)
    {
        std::cerr << "SphereODF.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
//    if (!m_attributesFlag){
//        std::cerr << "SphereODF.rendering(): not rendering because there isn't any sphere to render. Use method add(float radius, glm::vec3 center)." << std::endl;
//        return;
//    }
    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();
//    updateODFMapTexture();

//    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);

//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

#ifndef QT_NO_DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    glDrawElementsInstanced(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr, int(m_instancedSphereAttributes.size()));
    _check_gl_error(__FILE__,__LINE__);

//    glDisable(GL_DEPTH_TEST);
#ifndef QT_NO_DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    glDisable(GL_CULL_FACE);

    m_ODFMapTexture.deleteTexture();
    m_idxBuffer.unbind();
    m_vao.unbind();
    m_program.release();



}

void SphereODF::setVaoLayout()
{
    int locVertex = m_program.getAttribLocation("vertex");
    int locNormal = m_program.getAttribLocation("normal");

    m_vao.bind();

    m_vao.push<float>(locVertex, 3);
//    m_vao.push<float>(locNormal, 4);
//    m_vao.push<float>(-1, 2); //Reserved for texture, which is not being used
    m_vao.addBuffer(m_SphereVBO);
    m_vao.clearLayout(); //Clearing to support other buffer, that has the Center/Radius instanced parameters.

    int locDisplacement = m_program.getAttribLocation("instance_displacement");
    int locReorientMatrix = m_program.getAttribLocation("instance_reorientationMatrix");


    m_vao.push<float>(locDisplacement,     3, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix,   4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix+1, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix+2, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locReorientMatrix+3, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);

    m_vao.addBuffer(m_SphereAttributesVBO);
    m_vao.clearLayout();
    _check_gl_error(__FILE__,__LINE__);

}
//void SphereODF::setTextureCoords()
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

