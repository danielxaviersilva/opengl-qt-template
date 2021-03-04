#include "IcoSphere.h"

void IcoSphere::shrinkVecBaseAttributes(std::vector<baseSphereAttributes> &V, std::vector<unsigned int> &index, bool verbose)
{
    std::vector<int> indexChanges;
    indexChanges.resize(V.size());

    for (unsigned int i = 0; i < V.size(); i++)
        indexChanges[i] = i;

    if(verbose)
        std::cout << "Arg Vector Size: "<< V.size() << " -> ";
    int vSize = V.size();
    int idxFix = 0;

    for (int i = 0; i < vSize -1;i++)
        for (int j = i+1; j < vSize; j++)
            if (glm::dot(glm::vec3(V[i].vertex), glm::vec3(V[j].vertex)) == 1.0f)
                indexChanges[j] = i;

    for (int i = 0; i < vSize -1;i++)
        for (int j = i+1; j < vSize; j++)
            if (glm::dot(glm::vec3(V[i].vertex), glm::vec3(V[j].vertex)) == 1.0f){
                V.erase(V.begin() + j);
                for (auto & id: indexChanges)//unsigned int k = 0; k < indexChanges.size(); k++)
                    if (id > j)
                        id--;
                j--;
                vSize--;
            }

    for (unsigned int i = 0; i < index.size(); i++)
        index[i] = indexChanges[index[i]];

    if (verbose)
        std::cout << V.size() << std::endl;
}


IcoSphere::IcoSphere(int vertexAmount):
    m_vertexAmountThreshold(vertexAmount), m_initialized(false), m_attributesFlag(false)
{
    m_materialAmbientColor  = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    m_materialDiffuseColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    m_materialSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_shineness             = 5.0f;
}

void IcoSphere::addSphere(float r, glm::vec3 center, glm::vec3 axis)
{
    if(!m_initialized) {
        std::cerr << "IcoSphere.addSphere(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }

    if(r < 0.0f){
        std::cerr << "IcoSphere::addSphere(): for God's sake, dont send to me a negative radius (" << r << ")" << std::endl;
        return;
    }
    m_attributesFlag = true;
    m_sphereAttributes.push_back({r, center, shiftYtoAxisMatrix(axis)});
    m_SphereAttributesVBO.updateBufferData(m_sphereAttributes.data(), m_sphereAttributes.size()*sizeof(sphereAttributes));
    _check_gl_error(__FILE__,__LINE__);
}

void IcoSphere::initialize(LightSource* lightSource)
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
    std::cerr << "IcoSphere::initialize(): Class already initialized" << std::endl;

}

void IcoSphere::render()
{
    if (!m_initialized){
        std::cerr << "IcoSphere.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    if (!m_attributesFlag){
        std::cerr << "IcoSphere.rendering(): not rendering because there isn't any sphere to render. Use method add(float radius, glm::vec3 center)." << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElementsInstanced(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr, int(m_sphereAttributes.size()));
    _check_gl_error(__FILE__,__LINE__);



    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    m_idxBuffer.unbind();
    m_vao.unbind();
    m_program.release();
}

void IcoSphere::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void IcoSphere::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = m_program.getUniformLocation("u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void IcoSphere::setLightSource(LightSource *lightSource)
{
    m_lightSource = lightSource;
    m_lightSource->updateLightSource(&m_program);
}

void IcoSphere::setMaterialLighting(glm::vec4 materialAmbientColor, glm::vec4 materialDiffuseColor, glm::vec4 materialSpecularColor, float shineness)
{
    m_materialAmbientColor  = materialAmbientColor;
    m_materialDiffuseColor  = materialDiffuseColor;
    m_materialSpecularColor = materialSpecularColor;
    m_shineness             = shineness;
    updateMaterialLighting();
}


void IcoSphere::updateMaterialLighting()
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

void IcoSphere::setSphereSurface()
{
    const double t = (1.0 + std::sqrt(5.0)) / 2.0;
    std::vector <baseSphereAttributes> sphereAttributesBuffer;
    std::vector<unsigned int> idxSet;

    std::vector<glm::vec3> sphereVertex;
    sphereVertex.push_back(glm::normalize(glm::vec3(-1.0,  t, 0.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3( 1.0,  t, 0.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3(-1.0, -t, 0.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3( 1.0, -t, 0.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3(0.0, -1.0,  t)));
    sphereVertex.push_back(glm::normalize(glm::vec3(0.0,  1.0,  t)));
    sphereVertex.push_back(glm::normalize(glm::vec3(0.0, -1.0, -t)));
    sphereVertex.push_back(glm::normalize(glm::vec3(0.0,  1.0, -t)));
    sphereVertex.push_back(glm::normalize(glm::vec3( t, 0.0, -1.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3( t, 0.0,  1.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3(-t, 0.0, -1.0)));
    sphereVertex.push_back(glm::normalize(glm::vec3(-t, 0.0,  1.0)));


    // Vertices
    sphereAttributesBuffer = vertexListToVertexNormalAttributes(sphereVertex);
    // Faces
    idxSet = {0,11,5,0,5,1,
              0,1,7,0,7,10,
              0,10,11,1,5,9,
              5,11,4,11,10,2,
              10,7,6,7,1,8,
              3,9,4,3,4,2,
              3,2,6,3,6,8,
              3,8,9,4,9,5,
              2,4,11,6,2,10,
              8,6,7,9,8,1};


    int currentVertexIdx = sphereVertex.size();

    while(currentVertexIdx < m_vertexAmountThreshold){

        std::cout << "idxBufferSize: " << idxSet.size() << std::endl;
        std::cout << "vertexListSize: " << sphereVertex.size() << std::endl << std::endl;

        std::vector<unsigned int> idxAux;
        for (uint32_t i = 0; i < idxSet.size()/3; ++i)
        {
            const uint32_t f0 = idxSet[i * 3];
            const uint32_t f1 = idxSet[i * 3 + 1];
            const uint32_t f2 = idxSet[i * 3 + 2];

            const glm::vec3 v0 = glm::vec3(sphereVertex[f0]);
            const glm::vec3 v1 = glm::vec3(sphereVertex[f1]);
            const glm::vec3 v2 = glm::vec3(sphereVertex[f2]);


            glm::vec3 v01 = glm::normalize(0.5f*(v0 + v1));
            glm::vec3 v12 = glm::normalize(0.5f*(v1 + v2));
            glm::vec3 v20 = glm::normalize(0.5f*(v2 + v0));
            sphereVertex.push_back(v01);
            sphereVertex.push_back(v12);
            sphereVertex.push_back(v20);

            idxAux.push_back(f0);
            idxAux.push_back(currentVertexIdx);
            idxAux.push_back(currentVertexIdx+2);

            idxAux.push_back(currentVertexIdx);
            idxAux.push_back(f1);
            idxAux.push_back(currentVertexIdx+1);

            idxAux.push_back(currentVertexIdx+1);
            idxAux.push_back(f2);
            idxAux.push_back(currentVertexIdx+2);

            idxAux.push_back(currentVertexIdx);
            idxAux.push_back(currentVertexIdx+1);
            idxAux.push_back(currentVertexIdx+2);

            currentVertexIdx+=3;
        }
        idxSet = idxAux;

    }




    sphereAttributesBuffer = vertexListToVertexNormalAttributes(sphereVertex);


    shrinkVecBaseAttributes(sphereAttributesBuffer, idxSet, true);

     m_verticesSize = idxSet.size();
     m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(baseSphereAttributes));
     _check_gl_error(__FILE__,__LINE__);


        m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
        m_idxBuffer.unbind();
        _check_gl_error(__FILE__,__LINE__);
}


void IcoSphere::setVaoLayout()
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

std::vector<float> IcoSphere::computeIcosahedronVertices()
{
    const float PI = acos(-1);
    const float radius = 1.0f;
        const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

        std::vector<float> vertices(12 * 3);    // 12 vertices
        int i1, i2;                             // indices
        float z, xy;                            // coords
        float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 2nd row
        float hAngle2 = -PI / 2;                // start from -90 deg at 3rd row

        // the first top vertex (0, 0, r)
        vertices[0] = 0;
        vertices[1] = 0;
        vertices[2] = radius;

        // 10 vertices at 2nd and 3rd rows
        for(int i = 1; i <= 5; ++i)
        {
            i1 = i * 3;         // for 2nd row
            i2 = (i + 5) * 3;   // for 3rd row

            z = radius * sinf(V_ANGLE);             // elevaton
            xy = radius * cosf(V_ANGLE);

            vertices[i1] = xy * cosf(hAngle1);      // x
            vertices[i2] = xy * cosf(hAngle2);
            vertices[i1 + 1] = xy * sinf(hAngle1);  // x
            vertices[i2 + 1] = xy * sinf(hAngle2);
            vertices[i1 + 2] = z;                   // z
            vertices[i2 + 2] = -z;

            // next horizontal angles
            hAngle1 += H_ANGLE;
            hAngle2 += H_ANGLE;
        }

        // the last bottom vertex (0, 0, -r)
        i1 = 11 * 3;
        vertices[i1] = 0;
        vertices[i1 + 1] = 0;
        vertices[i1 + 2] = -radius;

        return vertices;

}

std::vector<IcoSphere::baseSphereAttributes> IcoSphere::vertexListToVertexNormalAttributes(const std::vector<glm::vec3> vertex)
{
    std::vector<baseSphereAttributes> out;
    out.reserve(vertex.size());

    for (auto &v : vertex)
        out.emplace_back(baseSphereAttributes(glm::vec4(v, 1.0f), glm::vec4(v, 0.0f)));

    return out;

}




//void IcoSphere::setTextureCoords()
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
