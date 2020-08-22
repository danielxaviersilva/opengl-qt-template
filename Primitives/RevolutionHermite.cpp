#include "RevolutionHermite.h"

RevolutionHermite::RevolutionHermite(): m_initialized(false)
{

}

void RevolutionHermite::setHermiteRevolution(std::vector<glm::vec2> controlPoints, int res, int thetaRes)
{
    std::vector<float> T;
    std::vector<float> meshX;
    std::vector<float> meshY;


    float ** hermitePolynomials = new float*[4];
    hermitePolynomials[0] = new float[res];
    hermitePolynomials[1] = new float[res];
    hermitePolynomials[2] = new float[res];
    hermitePolynomials[3] = new float[res];



    for (int i = 0; i < res; i++)
       T.push_back((float) i/((float)res - 1));

    for (int i = 0; i< res; i++)
    {
        hermitePolynomials[0][i] = 2*T[i]*T[i]*T[i] - 3*T[i]*T[i] + 1;
        hermitePolynomials[1][i] = -2*T[i]*T[i]*T[i] + 3*T[i]*T[i];
        hermitePolynomials[2][i] = T[i]*T[i]*T[i] - 2*T[i]*T[i] + T[i];
        hermitePolynomials[3][i] = T[i]*T[i]*T[i] - T[i]*T[i];
    }

    for (int i = 0; i< res; i++)
    {
        meshX.push_back(
        hermitePolynomials[0][i]*controlPoints[0].x +
        hermitePolynomials[1][i]*controlPoints[1].x +
        hermitePolynomials[2][i]*controlPoints[2].x +
        hermitePolynomials[3][i]*controlPoints[3].x);
        meshY.push_back(
        hermitePolynomials[0][i]*controlPoints[0].y +
        hermitePolynomials[1][i]*controlPoints[1].y +
        hermitePolynomials[2][i]*controlPoints[2].y +
        hermitePolynomials[3][i]*controlPoints[3].y);
    }

    std::vector<glm::vec4> line;


    for (int i = 0; i<res; i++)
        line.push_back(glm::vec4(meshX[i], meshY[i], 0.0f,1.0f));

    //Revolution

    float ** RevX = new float*[thetaRes];
        for (int i =0 ; i < thetaRes; i++)
            RevX[i] = new float[res];

        float ** RevY = new float*[thetaRes];
        for (int i =0 ; i < thetaRes; i++)
            RevY[i] = new float[res];

        float ** RevZ = new float*[thetaRes];
        for (int i =0 ; i < thetaRes; i++)
            RevZ[i] = new float[res];


        std::vector<float> theta;
        std::vector<glm::vec4> surface;

        for (int i = 0; i < thetaRes; i++)
            theta.push_back(2*M_PI*i/(thetaRes-1));

        for (int i = 0; i < thetaRes; i++)
        {
            for(int j = 0; j<line.size(); j++)
            {
                RevX[i][j] = line[j].x*(cos(theta[i])); //+ Z[j]*((float)Math.sin(theta[i]));
                RevY[i][j] = line[j].y;
                RevZ[i][j] = -line[j].x*((float)sin(theta[i]));// + Z[j]*((float)Math.cos(theta[i]));
            }
        }

        for (int i = 0; i < thetaRes-1; i++)
        {
            for(unsigned int j = 0; j<line.size() -1; j++)
            {
                surface.push_back(glm::vec4(RevX[i][j], RevY[i][j],RevZ[i][j], 1.0f));
                surface.push_back(glm::vec4(RevX[i+1][j], RevY[i+1][j],RevZ[i+1][j], 1.0f));
                surface.push_back(glm::vec4(RevX[i+1][j+1], RevY[i+1][j+1],RevZ[i+1][j+1], 1.0f));
                surface.push_back(glm::vec4(RevX[i][j], RevY[i][j],RevZ[i][j], 1.0f));
                surface.push_back(glm::vec4(RevX[i+1][j+1], RevY[i+1][j+1],RevZ[i+1][j+1], 1.0f));
                surface.push_back(glm::vec4(RevX[i][j+1], RevY[i][j+1],RevZ[i][j+1], 1.0f));
            }
        }

        m_verticesSize = surface.size();
        m_vbo.updateBufferData(surface.data(), surface.size()*sizeof(glm::vec4));


        for (int i =0 ; i < thetaRes; i++)
            delete[] RevX[i];


        for (int i =0 ; i < thetaRes; i++)
            delete[] RevY[i];


        for (int i =0 ; i < thetaRes; i++)
            delete[] RevZ[i];

        delete []hermitePolynomials[0];
        delete []hermitePolynomials[1];
        delete []hermitePolynomials[2];
        delete []hermitePolynomials[3];


        delete [] RevX;
        delete [] RevY;
        delete [] RevZ;
        delete []hermitePolynomials;









}

void RevolutionHermite::initialize()
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

void RevolutionHermite::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void RevolutionHermite::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void RevolutionHermite::setColor(glm::vec4 color)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort colorLoc = glGetUniformLocation(m_program.getProgramID(), "u_color");
        glUniform4fv( colorLoc, 1, glm::value_ptr(color));
        _check_gl_error(__FILE__, __LINE__);
    }

}

void RevolutionHermite::render()
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
