#include "QBallEvenRenderer.h"

QBallEvenRenderer::QBallEvenRenderer(): QBallRenderer()
{

}


void QBallEvenRenderer::initialize(QBall *qBall)
{
    m_InstancesCount = qBall->getInstancesAmount();
    m_qBallRef = qBall;
    if(!m_initialized) {
        m_initialized = true;
        m_program.loadProgram("./Models/renderQBallEvenGlyphs.vert","./Models/renderQBallEvenGlyphs.frag");
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

void QBallEvenRenderer::render(const std::vector<unsigned int>& odfIndexSet)
{
    if (!m_initialized)
    {
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


void QBallEvenRenderer::setInstancedVertexAttribBuffer(const std::vector<unsigned int>& odfIndexSet)
{
    //    Timer T(__FUNCTION__);

        std::vector<glm::mat4> currentReorientMatrixSet;
        std::vector<float> currentODFListSet;
    //    currentReorientMatrixSet.reserve(m_pts_from_tex_coord.m_num_points);
    //    currentODFListSet.reserve(m_pts_from_tex_coord.m_num_points*m_ODFsize);


        currentReorientMatrixSet.resize(odfIndexSet.size());
        currentODFListSet.resize(odfIndexSet.size()*m_ODFsize/2);

    #pragma omp parallel
        for(size_t i = 0; i < odfIndexSet.size(); i++) {

            float* currentVoxelAttributes = m_qBallRef->getVoxelODF(odfIndexSet[i]);
            int currentODFIndex = 0;
            for(int j = 0; j< m_ODFsize/2;j++) {
               currentODFListSet[i*m_ODFsize/2 + j] = currentVoxelAttributes[currentODFIndex++];
            }

            currentReorientMatrixSet[i] = m_qBallRef->getVoxelDisplacement(odfIndexSet[i]);
         }

        m_ODFMapTexture.uploadTexture(currentODFListSet, m_ODFsize/2, odfIndexSet.size());
        glUniform1i(m_program.getUniformLocation("u_ODFMap"), m_slot);
        m_ODFMapTexture.Bind(m_slot);
        m_ODFMapTexture.Unbind();

        m_SphereAttributesVBO.updateBufferData(currentReorientMatrixSet.data(), currentReorientMatrixSet.size()*sizeof(glm::mat4));
        _check_gl_error(__FILE__,__LINE__);
}
