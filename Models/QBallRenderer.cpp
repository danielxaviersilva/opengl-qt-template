#include "QBallRenderer.h"
#include <Utilities/Timer.h>

QBallRenderer::QBallRenderer(int theta, int phi): m_thetaRes(theta), m_phiRes(phi), m_initialized(false), m_slot(0)
{

}

void QBallRenderer::initialize(QBall *qBall)
{
    m_InstancesCount = qBall->getInstancesAmount();
    m_qBallRef = qBall;
    if(!m_initialized)
    {

        m_initialized = true;
        m_program.loadProgram("./Models/renderQBallGlyphs.vert","./Models/renderQBallGlyphs.frag");
        m_program.useProgram();

        m_SphereAttributesVBO.genBuffer();
        m_SphereVBO.genBuffer();
        m_idxBuffer.genBuffer();

        setVaoLayout();
        setSphereSurface(); //this method assign the m_SphereVBO buffer with a unitary sphere

        int glyphResolutionloc = m_program.getUniformLocation("u_glyphResolution");
        int scaleLoc = m_program.getUniformLocation("u_scale");
        float scale = m_qBallRef->getScale();
        std::cout << scale << std::endl;
        glUniform1f(glyphResolutionloc,m_ODFsize);
        glUniform1f(scaleLoc, scale);

        setProjectionMatrix(glm::mat4(1.0f));
        setMVMatrix(glm::mat4(1.0f));

        m_program.release();
    }
}

void QBallRenderer::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void QBallRenderer::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void QBallRenderer::setSphereSurface()
{
//    std::vector <baseSphereAttributes> sphereAttributesBuffer;
    std::vector <glm::vec3> sphereAttributesBuffer;

    std::vector<unsigned int> idxSet;
//    idxSet.reserve(6*m_thetaRes*(m_phiRes-1));
//    sphereAttributesBuffer.reserve(m_phiRes*m_thetaRes);
    sphereAttributesBuffer = m_qBallRef->m_sphereAttributesBuffer;
    idxSet = m_qBallRef->m_idxSet;
    //Interval of meshes:
    //phi: [-pi/2, pi/2]
    //theta: [0, 2pi)
    //The loop is designed to make a closed circle on theta,
    //The only repetition of elements on buffer are when phi = ±pi/2

//    for (int i = 0; i < m_phiRes; i++)
//    {
//        float phi =  M_PI/2 - M_PI*i/(m_phiRes - 1);
//        for(int j = 0; j<m_thetaRes; j++)
//        {
//             float theta = 2*(M_PI)*j/(m_thetaRes);

////             sphereAttributesBuffer.emplace_back(baseSphereAttributes(
////             glm::vec4(cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta), 1.0f), //vertex
////             glm::vec4(cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta), 0.0f))); //normals

//             sphereAttributesBuffer.emplace_back(glm::vec3
//             (cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta))); //normals

//              if (i < m_phiRes -1)
//              {
//                    idxSet.emplace_back(i*m_thetaRes + j                   );
//                    idxSet.emplace_back((i+1)*m_thetaRes + j               );
//                    idxSet.emplace_back((i+1)*m_thetaRes + (j+1)%m_thetaRes);
//                    idxSet.emplace_back(i*m_thetaRes + j                   );
//                    idxSet.emplace_back((i+1)*m_thetaRes + (j+1)%m_thetaRes);
//                    idxSet.emplace_back(i*m_thetaRes + (j+1)%m_thetaRes    );
//              }
//        }
//     }

        shrinkVec3(sphereAttributesBuffer, idxSet, true);
        m_ODFsize = sphereAttributesBuffer.size();

        m_verticesSize = idxSet.size();//sphereAttributesBuffer.size();

//        m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(baseSphereAttributes));
          m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(glm::vec3));
        _check_gl_error(__FILE__,__LINE__);


        m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
        m_idxBuffer.unbind();
        _check_gl_error(__FILE__,__LINE__);
}

void QBallRenderer::setInstancedVertexAttribBuffer()
{
//    Timer T(__FUNCTION__);

    std::vector<glm::mat4> currentReorientMatrixSet;
    std::vector<float> currentODFListSet;


//    currentReorientMatrixSet.reserve(m_pts_from_tex_coord.m_num_points);
//    currentODFListSet.reserve(m_pts_from_tex_coord.m_num_points*m_ODFsize);


    currentReorientMatrixSet.resize(m_InstancesCount);
    currentODFListSet.resize(m_ODFsize*m_InstancesCount);

#pragma omp parallel
    for(unsigned int i = 0; i < m_InstancesCount; i++)
    {

//            float currentGFA = volumeQBall->getVoxelGFA(currentVoxelIndex);

        float* currentVoxelAttributes = m_qBallRef->getVoxelODF(i);
        int currentODFIndex = 0;
        for(int j = 0; j< m_ODFsize;j++){
           currentODFListSet[i*m_ODFsize + j] = (currentVoxelAttributes[currentODFIndex++]);
            }

        currentReorientMatrixSet[i] = m_qBallRef->getVoxelDisplacement(i);
      }

    m_ODFMapTexture.uploadTexture(currentODFListSet, m_ODFsize, m_InstancesCount);
    int instanceCountLoc = m_program.getUniformLocation("u_instanceCount");
    glUniform1f(instanceCountLoc , float(m_InstancesCount));
    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);
    m_ODFMapTexture.Unbind();

    m_SphereAttributesVBO.updateBufferData(currentReorientMatrixSet.data(), currentReorientMatrixSet.size()*sizeof(glm::mat4));
    _check_gl_error(__FILE__,__LINE__);

}

void QBallRenderer::setVaoLayout()
{
    int locVertex = m_program.getAttribLocation("vertex");
//    int locNormal = m_program.getAttribLocation("normal");

    m_vao.bind();

    m_vao.push<float>(locVertex, 3);
//    m_vao.push<float>(locNormal, 4);
//    m_vao.push<float>(-1, 2); //Reserved for texture, which is not being used
    m_vao.addBuffer(m_SphereVBO);
    m_vao.clearLayout(); //Clearing to support other buffer, that has the Center/Radius instanced parameters.




    int locDisplacement = m_program.getAttribLocation("instance_displacement");



////    m_vao.push<float>(locDisplacement,     3, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locDisplacement,   4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locDisplacement+1, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locDisplacement+2, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);
    m_vao.push<float>(locDisplacement+3, 4, GL_FALSE, VertexArray::INSTANTIATION_MODE::INSTANCED, 1);

    m_vao.addBuffer(m_SphereAttributesVBO);
    m_vao.clearLayout();
    _check_gl_error(__FILE__,__LINE__);
}

void QBallRenderer::render()
{

    if (!m_initialized)
    {
        std::cerr << "SphereODF.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }

    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();
    setInstancedVertexAttribBuffer();
//    updateODFMapTexture();

    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);

//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

#ifndef QT_NO_DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    glDrawElementsInstanced(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr, m_InstancesCount);
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


