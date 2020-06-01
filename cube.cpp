#include "cube.h"

Cube::Cube(float edge, glm::vec3 center, glm::vec3 rotationAxis, float angleRotation):
             m_edge         (edge),
             m_center       (center),
             m_rotationAxis (rotationAxis),
             m_angleRotation(angleRotation)
{

}

void Cube::initialize()
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
           {
               std::cerr << "Cube::initialize: At least one of the input names couldn't be found" << std::endl;
           }

           m_vao.bind();

           m_vao.push<float>(locVertex, 4);
           m_vao.push<float>(locNormal, 4);
   //        m_vao.push<float>(-1, 2); //Reserved for texture, which is not being used

           //Calculate the vertices and assign to buffer m_vbo
           setCubeSurface();

           m_vao.addBuffer(m_vbo);
           setLighting();

           //create vertices of a sphere centered at the origin
   //        superquadric_coord = new SuperquadricCoord(position_loc, new GLfloat[4]{1.f, 0, 0, 0}, 5, 5);


           m_vao.unbind();
           m_program.release();
           _check_gl_error(__FILE__, __LINE__);

       }

}

void Cube::setCubeSurface()
{

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3(-1.0,-1.0,-1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3(-1.0,-1.0,-1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3(-1.0,-1.0, 1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3(-1.0,-1.0, 1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3(-1.0, 1.0,-1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3(-1.0, 1.0,-1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3(-1.0, 1.0, 1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3(-1.0, 1.0, 1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3( 1.0,-1.0,-1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3( 1.0,-1.0,-1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3( 1.0,-1.0, 1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3( 1.0,-1.0, 1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3( 1.0, 1.0,-1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3( 1.0, 1.0,-1.0f)), 0.0f))); //normals

    m_cubeAttributes.push_back(cubeAttributes(
    glm::vec4(glm::vec3( 1.0, 1.0, 1.0f)*(m_edge/2), 1.0f), //vertex
    glm::vec4(glm::normalize(glm::vec3( 1.0, 1.0, 1.0f)), 0.0f))); //normals

        unsigned int idxSet[] = {0,1,2,
                                 2,1,3,
                                 4,6,5,
                                 5,6,7,
                               0,4,1,
                               1,4,5,
                               2,3,6,
                               6,3,7,
                               0,2,4,
                               4,2,6,
                               1,5,3,
                               3,5,7}; //reference to the vertex in the way it makes triangles

        m_idxBuffer.updateBufferData(idxSet, (unsigned int)sizeof(idxSet)/sizeof(unsigned int));
        m_idxBuffer.unbind();

            m_verticesSize = m_cubeAttributes.size();
            m_vbo.updateBufferData(m_cubeAttributes.data(), m_cubeAttributes.size()*sizeof(cubeAttributes));
            _check_gl_error(__FILE__,__LINE__);


}

void Cube::render()
{
    if (!m_initialized)
    {
        std::cerr << "Cube.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();
    m_idxBuffer.bind();
    glEnable(GL_CULL_FACE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    glDisable(GL_CULL_FACE);
    m_idxBuffer.unbind();
    m_program.release();
    m_vao.unbind();

}

void Cube::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void Cube::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void Cube::setLighting()
{
    //ref to category of how models are computed. 0,1 or 2.
    float shading_model       = 1.0f;
    //Light source Parameters
    glm::vec4 light_location      = glm::vec4(0.0f,0.0,1.0,1.0);
    glm::vec4 spot_direction      = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
    float     spot_exponent       = 25.0f;
    float     spot_cutoff         = 180.0f;
    glm::vec4 Light_ambientColor  = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    glm::vec4 Light_diffuseColor  = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
    glm::vec4 Light_specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //Material Parameters
    glm::vec4 Mat_ambientColor    = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 Mat_diffuseColor    = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 Mat_specularColor   = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float     shineness           = 1.0f;
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

void Cube::_check_gl_error(const char *file, int line) {
    //Ting: sugestao para chamada
    //_check_gl_error(__FILE__,__LINE__)
    GLenum err (glGetError());
    while(err!=GL_NO_ERROR) {
        std::string error;
        switch(err) {
            case GL_INVALID_OPERATION: error="INVALID_OPERATION"; break;
            case GL_INVALID_ENUM: error="INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error="INVALID_VALUE"; break;
            case GL_OUT_OF_MEMORY: error="OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error="INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_STACK_UNDERFLOW: error="GL_STACK_UNDERFLOW"; break;
            case GL_STACK_OVERFLOW: error="GL_STACK_OVERFLOW"; break;
        }
        std::cout << "GL_" << error.c_str() <<" - "<< file << ":" << line << std::endl;
        err=glGetError();
    }
}
