#include "QBallRenderer.h"
#include <Utilities/Timer.h>
#include <glm/gtx/string_cast.hpp>

QBallRenderer::QBallRenderer(): m_initialized(false), m_slot(0)
{}

void QBallRenderer::initialize(QBall *qBall)
{
    m_InstancesCount = qBall->getInstancesAmount();
    m_qBallRef = qBall;
    if(!m_initialized) {
        m_initialized = true;
        m_program.loadProgram("./Models/renderQBallGlyphs.vert","./Models/renderQBallGlyphs.frag");
        m_program.useProgram();

        m_SphereAttributesVBO.genBuffer();
        m_SphereVBO.genBuffer();
        m_idxBuffer.genBuffer();

        setVaoLayout();
        setSphereSurface(); //this method assign the m_SphereVBO buffer with a unitary sphere

//        int glyphResolutionloc = m_program.getUniformLocation("u_glyphResolution");
        float scale = m_qBallRef->getScale();
        int scaleLoc = m_program.getUniformLocation("u_scale");

//        std::cout << scale << std::endl;
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
    std::vector <glm::vec3> sphereAttributesBuffer = m_qBallRef->getBaseDirections();
    std::vector<unsigned int> idxSet = m_qBallRef->getIndexBuffer();

    m_ODFsize = sphereAttributesBuffer.size();
    m_verticesSize = idxSet.size();
    m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(glm::vec3));
    _check_gl_error(__FILE__,__LINE__);


    m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
    m_idxBuffer.unbind();
    _check_gl_error(__FILE__,__LINE__);
}

void QBallRenderer::setInstancedVertexAttribBuffer(const std::vector<unsigned int>& odfIndexSet)
{
//    Timer T(__FUNCTION__);

    std::vector<glm::mat4> currentReorientMatrixSet;
    std::vector<float> currentODFListSet;
//    currentReorientMatrixSet.reserve(m_pts_from_tex_coord.m_num_points);
//    currentODFListSet.reserve(m_pts_from_tex_coord.m_num_points*m_ODFsize);


    currentReorientMatrixSet.resize(odfIndexSet.size());
    currentODFListSet.resize(m_ODFsize*odfIndexSet.size());

#pragma omp parallel
    for(size_t i = 0; i < odfIndexSet.size(); i++) {

        float* currentVoxelAttributes = m_qBallRef->getVoxelODF(odfIndexSet[i]);
        int currentODFIndex = 0;
        for(int j = 0; j< m_ODFsize;j++) {
           currentODFListSet[i*m_ODFsize + j] = currentVoxelAttributes[currentODFIndex++];
        }

        currentReorientMatrixSet[i] = m_qBallRef->getVoxelDisplacement(odfIndexSet[i]);
     }

    m_ODFMapTexture.uploadTexture(currentODFListSet, m_ODFsize, odfIndexSet.size());
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

void QBallRenderer::render(const std::vector<unsigned int>& odfIndexSet)
{

    if (!m_initialized) {
        std::cerr << "SphereODF.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }

    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();
    setInstancedVertexAttribBuffer(odfIndexSet);

    glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
    m_ODFMapTexture.Bind(m_slot);

//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

#ifndef QT_NO_DEBUG
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    glDrawElementsInstanced(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr, odfIndexSet.size());
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


