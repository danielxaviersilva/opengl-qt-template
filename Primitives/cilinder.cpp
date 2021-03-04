#include "cilinder.h"

#include <glm/gtx/string_cast.hpp>

Cilinder::Cilinder(int thetaRes, int heightRes, float R, float height, glm::vec3 center, glm::vec3 mainAxis):
m_thetaRes(thetaRes), m_heightRes(heightRes), m_height(height), m_radius(R), m_center(center), m_mainAxis(mainAxis)
{}

void Cilinder::initialize()
{
    if(!m_initialized)
    {
        m_initialized = true;
//        Carregar o shader
        m_program.loadProgram("./renderLightning.vert","./renderLightning.frag");
        m_program.useProgram();

        int locVertex =glGetAttribLocation(m_program.getProgramID(),"vertex");
        int locNormal = glGetAttribLocation(m_program.getProgramID(),"v_normal");
        if(locVertex == -1 || locNormal == -1)
            std::cerr << "Cilinder::initialize: At least one of the input names couldn't be found" << std::endl;

        m_vao.bind();
        m_vao.push<float>(locVertex, 4);
        m_vao.push<float>(locNormal, 4);
//        m_vao.push<float>(-1, 2); //Reserved for texture, which is not being used
        //Calculate the vertices and assign to buffer m_vbo
        setCilinderSurface();
        m_vao.addBuffer(m_vbo);


        setLighting();
        m_vao.unbind();
        m_program.release();
    }
}

void Cilinder::setCilinderSurface()
{
    std::vector<cilinderAttributes> cilinderAttrib;
    float * thetaGrid;
    float ** X;
    float ** Y;
    float ** Z;

    thetaGrid = new float [m_thetaRes];
    X = new float* [m_heightRes];
    Y = new float *[m_heightRes];
    Z = new float *[m_heightRes];

    for (int i = 0; i < m_thetaRes; i++)
        thetaGrid[i] = 2*(M_PI)*i/(m_thetaRes);

    for (int count = 0; count < m_heightRes; count++)
    {
        X[count] = new float [m_thetaRes];
        Y[count] = new float [m_thetaRes];
        Z[count] = new float [m_thetaRes];
    }

    //Grids: theta: [0, 2pi), Height: [-h/2, h/2]
    for (int i = 0; i< m_heightRes; i++)
        for (int j = 0; j< m_thetaRes; j++)
        {
            X[i][j] = m_radius*(cos(2*(M_PI)*j/(m_thetaRes)));
            Y[i][j] = -m_height/2 + m_height*i/(m_heightRes - 1);
            Z[i][j] = m_radius*(sin(2*(M_PI)*j/(m_thetaRes)));
        }

    glm::mat4 tMat = shiftYtoAxisMatrix(m_mainAxis);
    tMat = glm::translate(tMat, m_center);
    glm::mat4 tMatNormal = glm::transpose(glm::inverse(tMat));

    std::vector<unsigned int> idxSet;
    idxSet.reserve(6*m_thetaRes*(m_heightRes - 1));
    for (int i = 0; i < m_heightRes; i++)
           for(int j = 0; j<m_thetaRes; j++)
           {
               cilinderAttrib.push_back(cilinderAttributes(
               tMat*glm::vec4(X[i][j], Y[i][j], Z[i][j], 1.0f), //vertex
               tMatNormal*glm::vec4(glm::normalize(glm::vec3(X[i][j], Y[i][j], Z[i][j])), 0.0f))); //normals

               if (i < m_heightRes -1) {
                     idxSet.emplace_back(i*m_thetaRes + j                   );
                     idxSet.emplace_back((i+1)*m_thetaRes + j               );
                     idxSet.emplace_back((i+1)*m_thetaRes + (j+1)%m_thetaRes);
                     idxSet.emplace_back(i*m_thetaRes + j                   );
                     idxSet.emplace_back((i+1)*m_thetaRes + (j+1)%m_thetaRes);
                     idxSet.emplace_back(i*m_thetaRes + (j+1)%m_thetaRes    );
               }
           }
        m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
        m_idxBuffer.unbind();


        //Circle Upper part
        cilinderAttrib.push_back(cilinderAttributes(
        tMat*glm::vec4(0.0f, m_height/2, 0.0f, 1.0f),
        tMat*glm::vec4(0.0f, 1.0f, 0.0f , 0.0f)));

        for (int i = m_thetaRes; i >= 0 ; i--)
           cilinderAttrib.push_back(cilinderAttributes(
           tMat*glm::vec4(m_radius*cos(thetaGrid[i%m_thetaRes]), m_height/2, m_radius*sin(thetaGrid[i%m_thetaRes]), 1.0f),
           tMatNormal*glm::vec4(glm::normalize(glm::vec3(m_radius*cos(thetaGrid[i%m_thetaRes]), m_height/2, m_radius*sin(thetaGrid[i%m_thetaRes]))) , 0.0f)));

       //Circle bottom part

           cilinderAttrib.push_back(cilinderAttributes(
           tMat*glm::vec4(0.0f, -m_height/2, 0.0f, 1.0f),
           tMat*glm::vec4(0.0f, -1.0f, 0.0f , 0.0f)));

           for (int i = 0; i <= m_thetaRes; i++)
               cilinderAttrib.push_back(cilinderAttributes(
               tMat*glm::vec4(m_radius*cos(thetaGrid[i%m_thetaRes]), -m_height/2, m_radius*sin(thetaGrid[i%m_thetaRes]), 1.0f),
               tMatNormal*glm::vec4(glm::normalize(glm::vec3(m_radius*cos(thetaGrid[i%m_thetaRes]), -m_height/2, m_radius*sin(thetaGrid[i%m_thetaRes]))) , 0.0f)));

           for (int count = 0; count < m_heightRes; count++) {
               delete [] X[count];
               delete [] Y[count];
               delete [] Z[count];
           }
           delete[] X;
           delete[] Y;
           delete[] Z;
           delete [] thetaGrid;
        m_vbo.updateBufferData(cilinderAttrib.data(), cilinderAttrib.size()*sizeof(cilinderAttributes));
        _check_gl_error(__FILE__,__LINE__);
}

void Cilinder::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void Cilinder::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void Cilinder::render()
{
    if (!m_initialized) {
        std::cerr << "Cilinder.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();

    glEnable(GL_CULL_FACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDrawArrays(GL_TRIANGLES, 0, m_verticesSize);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6*m_thetaRes*(m_heightRes-1),  GL_UNSIGNED_INT, nullptr); //draw side
    _check_gl_error(__FILE__,__LINE__);
    m_idxBuffer.unbind();

    glDrawArrays(GL_TRIANGLE_FAN, m_thetaRes*m_heightRes, m_thetaRes+2 ); //draw upper part
    glDrawArrays(GL_TRIANGLE_FAN, m_thetaRes*m_heightRes+ m_thetaRes+2, m_thetaRes+2); //draw bottom parte
    glDisable(GL_CULL_FACE);

    m_idxBuffer.unbind();
    m_program.release();
    m_vao.unbind();
}

void Cilinder::setLighting()
{
//    //ref to category of how models are computed. 0,1 or 2.
//    float shading_model       = 1.0f;
//    //Light source Parameters
//    glm::vec4 light_location      = glm::vec4(-1.0f,1.0,0.0,1.0);
//    glm::vec4 spot_direction      = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
//    float     spot_exponent       = 25.0f;
//    float     spot_cutoff         = 180.0f;
//    glm::vec4 Light_ambientColor  = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
//    glm::vec4 Light_diffuseColor  = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
//    glm::vec4 Light_specularColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
//    //Material Parameters
//    glm::vec4 Mat_ambientColor    = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
//    glm::vec4 Mat_diffuseColor    = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
//    glm::vec4 Mat_specularColor   = glm::vec4(.0f, .0f, 1.0f, 1.0f);
//    float     shineness           = 1.0f;
//    glm::vec4 expectator_position = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

    //ref to category of how models are computed. 0,1 or 2.
    float shading_model       = 1.0f;
    //Light source Parameters
    glm::vec4 light_location      = glm::vec4(-1.0f,1.0,0.0,1.0);
    glm::vec4 spot_direction      = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
    float     spot_exponent       = 25.0f;
    float     spot_cutoff         = 180.0f;
    glm::vec4 Light_ambientColor  = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    glm::vec4 Light_diffuseColor  =  glm::vec4(0.6f, 0.3f, 0.3f, 1.0f);
    glm::vec4 Light_specularColor = glm::vec4(.4f, .4f, .4f, 1.0f);
    //Material Parameters
    glm::vec4 Mat_ambientColor    = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 Mat_diffuseColor    = glm::vec4(1.0f, .0f, 0.0f, 1.0f);
    glm::vec4 Mat_specularColor   = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float     shineness           = 5.0f;
    glm::vec4 expectator_position = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    m_program.useProgram();

    GLshort u_shading_modelLoc      = glGetUniformLocation(m_program.getProgramID(), "u_shading_model");
    //Light source Parameters
    GLshort u_light_locationLoc      = glGetUniformLocation(m_program.getProgramID(),"u_light_location"     );
    GLshort u_spot_directionLoc      = glGetUniformLocation(m_program.getProgramID(),"u_spot_direction"     );
    GLshort u_spot_exponentLoc       = glGetUniformLocation(m_program.getProgramID(),"u_spot_exponent"      );
    GLshort u_spot_cutoffLoc         = glGetUniformLocation(m_program.getProgramID(),"u_spot_cutoff"        );
    GLshort u_Light_ambientColorLoc  = glGetUniformLocation(m_program.getProgramID(),"u_Light_ambientColor" );
    GLshort u_Light_diffuseColorLoc  = glGetUniformLocation(m_program.getProgramID(),"u_Light_diffuseColor" );
    GLshort u_Light_specularColorLoc = glGetUniformLocation(m_program.getProgramID(),"u_Light_specularColor");
    //Material Parameters
    GLshort u_Mat_ambientColorLoc    = glGetUniformLocation(m_program.getProgramID(), "u_Mat_ambientColor");
    GLshort u_Mat_diffuseColorLoc    = glGetUniformLocation(m_program.getProgramID(), "u_Mat_diffuseColor");
    GLshort u_Mat_specularColorLoc   = glGetUniformLocation(m_program.getProgramID(), "u_Mat_specularColor");
    GLshort u_shinenessLoc           = glGetUniformLocation(m_program.getProgramID(), "u_shineness");
    GLshort u_expectator_positionLoc = glGetUniformLocation(m_program.getProgramID(), "u_expectator_position");

    if(u_shading_modelLoc   == -1 || u_light_locationLoc     == -1 || u_spot_directionLoc     == -1 ||
    u_spot_exponentLoc      == -1 || u_spot_cutoffLoc        == -1 || u_Light_ambientColorLoc == -1 ||
    u_Light_diffuseColorLoc == -1 || u_Light_specularColorLoc== -1 || u_Mat_ambientColorLoc   == -1 ||
    u_Mat_diffuseColorLoc   == -1 || u_Mat_specularColorLoc  == -1 || u_shinenessLoc          == -1 ||
    u_expectator_positionLoc == -1)
    {
        std::cerr << "At least one of the lightning parameters wasn't located in Shader" << std::endl;
    }

    glUniform1f (u_shading_modelLoc      ,  shading_model                      );
    glUniform4fv(u_light_locationLoc     ,1,glm::value_ptr(light_location     ));
    glUniform4fv(u_spot_directionLoc     ,1,glm::value_ptr(spot_direction     ));
    glUniform1f (u_spot_exponentLoc      ,  spot_exponent                      );
    glUniform1f (u_spot_cutoffLoc        ,  spot_cutoff                        );
    glUniform4fv(u_Light_ambientColorLoc ,1,glm::value_ptr(Light_ambientColor ));
    glUniform4fv(u_Light_diffuseColorLoc ,1,glm::value_ptr(Light_diffuseColor ));
    glUniform4fv(u_Light_specularColorLoc,1,glm::value_ptr(Light_specularColor));
    glUniform4fv(u_Mat_ambientColorLoc   ,1,glm::value_ptr(Mat_ambientColor   ));
    glUniform4fv(u_Mat_diffuseColorLoc   ,1,glm::value_ptr(Mat_diffuseColor   ));
    glUniform4fv(u_Mat_specularColorLoc  ,1,glm::value_ptr(Mat_specularColor  ));
    glUniform1f (u_shinenessLoc          ,  shineness                          );
    glUniform4fv(u_expectator_positionLoc,1,glm::value_ptr(expectator_position));
}

