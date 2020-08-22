#include "BicubicBezierSurface.h"

BicubicBezierSurface::BicubicBezierSurface() : m_initialized(false)
{

}

void BicubicBezierSurface::setBicubicBezierSurface(std::vector<glm::vec3> controlPoints, int res)
{
    std::vector<float> T;
    std::vector<float> S;


    float ** tBezierPolynomials = new float*[4];
    tBezierPolynomials[0] = new float[res];
    tBezierPolynomials[1] = new float[res];
    tBezierPolynomials[2] = new float[res];
    tBezierPolynomials[3] = new float[res];

    float ** sBezierPolynomials = new float*[4];
    sBezierPolynomials[0] = new float[res];
    sBezierPolynomials[1] = new float[res];
    sBezierPolynomials[2] = new float[res];
    sBezierPolynomials[3] = new float[res];

    float ** meshX = new float*[res];
    for (int i =0 ; i < res; i++)
        meshX[i] = new float[res];

    float ** meshY = new float*[res];
    for (int i =0 ; i < res; i++)
        meshY[i] = new float[res];

    float ** meshZ = new float*[res];
    for (int i =0 ; i < res; i++)
        meshZ[i] = new float[res];

    for (int i = 0; i < res; i++)
    {
        T.push_back(i/((float)res - 1));
        S.push_back((float) i/((float)res - 1));
    }

    for (int i = 0; i< res; i++)
    {
        tBezierPolynomials[3][i] = T[i]*T[i]*T[i];
        tBezierPolynomials[2][i] = 3*T[i]*T[i]*(1.0f - T[i]);
        tBezierPolynomials[1][i] = 3*T[i]*(1.0f -T[i])*(1.0f - T[i]);
        tBezierPolynomials[0][i] = (1.0f - T[i])*(1.0f - T[i])*(1.0f - T[i]);

        sBezierPolynomials[3][i] = S[i]*S[i]*S[i];
        sBezierPolynomials[2][i] = 3*S[i]*S[i]*(1.0f - S[i]);
        sBezierPolynomials[1][i] = 3*S[i]*(1.0f -S[i])*(1.0f - S[i]);
        sBezierPolynomials[0][i] = (1.0f - S[i])*(1.0f - S[i])*(1.0f - S[i]);
    }



    for (int i = 0; i< res; i++)
    {
        for (int j = 0; j<res; j++)
        {
            meshX[i][j] = 0.0f;
            meshY[i][j] = 0.0f;
            meshZ[i][j] = 0.0f;
        }
    }



    for (int i = 0; i< res; i++)
    {
        for (int j = 0; j<res; j++)
        {
        for(int p = 0; p<16; p++)
            {
                meshX[i][j] = meshX[i][j] + sBezierPolynomials[p%4][j]*controlPoints[p].x*tBezierPolynomials[p/4][i];
                meshY[i][j] = meshY[i][j] + sBezierPolynomials[p%4][j]*controlPoints[p].y*tBezierPolynomials[p/4][i];
                meshZ[i][j] = meshZ[i][j] + sBezierPolynomials[p%4][j]*controlPoints[p].z*tBezierPolynomials[p/4][i];
            }
        }
    }

    std::vector<glm::vec4> surface;

    for (int u = 0; u < res-1; u++)
    {
        for (int v = 0; v < res-1; v++)
        {
            surface.push_back(glm::vec4(meshX[u][v],meshY[u][v],meshZ[u][v],1.0f));
            surface.push_back(glm::vec4(meshX[u+1][v],meshY[u+1][v],meshZ[u+1][v],1.0f));
            surface.push_back(glm::vec4(meshX[u+1][v+1],meshY[u+1][v+1],meshZ[u+1][v+1],1.0f));
            surface.push_back(glm::vec4(meshX[u][v],meshY[u][v],meshZ[u][v],1.0f));
            surface.push_back(glm::vec4(meshX[u][v],meshY[u][v+1],meshZ[u][v+1],1.0f));
            surface.push_back(glm::vec4(meshX[u+1][v+1],meshY[u+1][v+1],meshZ[u+1][v+1],1.0f));

        }
    }



    m_verticesSize = surface.size();
    m_vbo.updateBufferData(surface.data(), surface.size()*sizeof(glm::vec4));


    for (int i =0 ; i < res; i++)
        delete[] meshX[i];


    for (int i =0 ; i < res; i++)
        delete[] meshY[i];


    for (int i =0 ; i < res; i++)
        delete[] meshZ[i];


    delete []tBezierPolynomials[0];
    delete []tBezierPolynomials[1];
    delete []tBezierPolynomials[2];
    delete []tBezierPolynomials[3];
    delete []sBezierPolynomials[0];
    delete []sBezierPolynomials[1];
    delete []sBezierPolynomials[2];
    delete []sBezierPolynomials[3];

    delete [] meshX;
    delete [] meshY;
    delete [] meshZ;
    delete []tBezierPolynomials;
    delete []sBezierPolynomials;



}

void BicubicBezierSurface::initialize()
{

    if(!m_initialized)
    {

        m_initialized = true;
        m_program.loadProgram("./SolidObjectDrawer.vert","./SolidObjectDrawer.frag");
        m_program.useProgram();

        int locVertex =glGetAttribLocation(m_program.getProgramID(),"vertex");

        if(locVertex == -1)
        {
            std::cerr << "BicubicBezier::initialize: At least one of the input names couldn't be found" << std::endl;
        }

        m_vao.bind();

        m_vao.push<float>(locVertex, 4);
        //Calculate the vertices and assign to buffer m_vbo
        m_vbo.genBuffer();

        m_vao.addBuffer(m_vbo);
        m_vao.unbind();
        m_program.release();
    }


}

void BicubicBezierSurface::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }


}

void BicubicBezierSurface::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void BicubicBezierSurface::setColor(glm::vec4 color)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort colorLoc = glGetUniformLocation(m_program.getProgramID(), "u_color");
        glUniform4fv( colorLoc, 1, glm::value_ptr(color));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void BicubicBezierSurface::render()
{
    if (!m_initialized)
    {
        std::cerr << "Sphere.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();

//    glEnable(GL_CULL_FACE);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDrawElements(GL_TRIANGLES, m_verticesSize,  GL_UNSIGNED_INT, nullptr);
    _check_gl_error(__FILE__,__LINE__);
//    glDrawArraysInstanced(GL_TRIANGLES, 0, m_verticesSize);
    glDrawArrays(GL_TRIANGLES, 0, m_verticesSize);

    glDisable(GL_CULL_FACE);

    m_vao.unbind();
    m_program.release();

}
