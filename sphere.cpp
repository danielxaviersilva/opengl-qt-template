#include "sphere.h"

//Sphere::Sphere(int theta, int phi, float R, float xc, float yc, float zc)
//{
//    m_thetaRes = theta;
//    m_phiRes = phi;
//    m_radius = R;

//    m_center[0] = xc;
//    m_center[1] = yc;
//    m_center[2] = zc;

//    m_initialized = false;

////    setSphereSurface();

//    //setTextureCoords();

//    bindSphere();
//}

Sphere::Sphere(int theta, int phi, float R, glm::vec3 center)
{
    m_thetaRes = theta;
    m_phiRes = phi;
    m_radius = R;
    m_center = center;
}

void Sphere::initialize()
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
            std::cerr << "Sphere::initialize: At least one of the input names couldn't be found" << std::endl;
        }

        m_vao.bind();

        m_vao.push<float>(locVertex, 4);
        m_vao.push<float>(locNormal, 4);
//        m_vao.push<float>(-1, 2); //Reserved for texture, which is not being used

        //Calculate the vertices and assign to buffer m_vbo
        setSphereSurface();

        m_vao.addBuffer(m_vbo);

        setLighting();

        //create vertices of a sphere centered at the origin
//        superquadric_coord = new SuperquadricCoord(position_loc, new GLfloat[4]{1.f, 0, 0, 0}, 5, 5);


        m_vao.unbind();
        m_program.release();

    }

}

float Sphere::getRadius() const
{
    return m_radius;
}

void Sphere::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_projectionMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void Sphere::setMVMatrix(glm::mat4 mvMatrix)
{
    if(m_initialized)
    {
        m_program.useProgram();
        GLshort mvMatrixLoc = glGetUniformLocation(m_program.getProgramID(), "u_modelViewMatrix");
        glUniformMatrix4fv( mvMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        _check_gl_error(__FILE__, __LINE__);
    }
}

void Sphere::setSphereSurface()
{
    float * phi;
    float * theta;

    phi = new float[m_phiRes];
    theta = new float [m_thetaRes];

    float ** X;
    float ** Y;
    float ** Z;

    X = new float*[m_phiRes];
    Y = new float *[m_phiRes];
    Z = new float *[m_phiRes];

    for (int count = 0; count < m_phiRes; count++)
    {
        X[count] = new float [m_thetaRes];
        Y[count] = new float [m_thetaRes];
        Z[count] = new float [m_thetaRes];
    }

    for (int i = 0; i < m_thetaRes; i++)
        theta[i] = 2*(M_PI)*i/(m_thetaRes - 1);

    for (int i = 0; i < m_phiRes; i++)
        phi[i] = M_PI/2 - M_PI*i/(m_phiRes - 1);

    for (int i = 0; i< m_phiRes; i++)
    {
        for (int j = 0; j< m_thetaRes; j++)
        {
            X[i][j] = m_radius*(cos(phi[i])*cos(theta[j]));
            Y[i][j] = m_radius*(cos(phi[i])*sin(theta[j]));
            Z[i][j] = m_radius*(sin(phi[i]));
        }
    }
    for (int i = 0; i < m_phiRes-1; i++)
    {
        for(int j = 0; j<m_thetaRes -1; j++)
        {
          m_sphereAttributes.push_back(sphereAttributes(
          glm::vec4(X[i+1][j] + m_center[0], Y[i+1][j] + m_center[1], Z[i+1][j] + m_center[2], 1.0f),
          glm::vec4(glm::normalize(glm::vec3(X[i+1][j], Y[i+1][j], Z[i+1][j])), 0.0f)));

          m_sphereAttributes.push_back(sphereAttributes(
          glm::vec4(X[i][j] + m_center[0], Y[i][j] + m_center[1], Z[i][j] + m_center[2], 1.0f), //vertex
          glm::vec4(glm::normalize(glm::vec3(X[i][j], Y[i][j], Z[i][j])), 0.0f))); //normals

          m_sphereAttributes.push_back(sphereAttributes(
          glm::vec4(X[i+1][j+1] + m_center[0], Y[i+1][j+1] + m_center[1], Z[i+1][j+1] + m_center[2], 1.0f), //vertex
          glm::vec4(glm::normalize(glm::vec3(X[i+1][j+1], Y[i+1][j+1], Z[i+1][j+1])), 0.0f))); //normals

          m_sphereAttributes.push_back(sphereAttributes(
          glm::vec4(X[i+1][j+1] + m_center[0], Y[i+1][j+1] + m_center[1], Z[i+1][j+1] + m_center[2], 1.0f), //vertex
          glm::vec4(glm::normalize(glm::vec3(X[i+1][j+1], Y[i+1][j+1], Z[i+1][j+1])), 0.0f))); //normals

          m_sphereAttributes.push_back(sphereAttributes(
          glm::vec4(X[i][j] + m_center[0], Y[i][j] + m_center[1], Z[i][j] + m_center[2], 1.0f), //vertex
          glm::vec4(glm::normalize(glm::vec3(X[i][j], Y[i][j], Z[i][j])), 0.0f))); //normals

          m_sphereAttributes.push_back(sphereAttributes(
          glm::vec4(X[i][j+1] + m_center[0], Y[i][j+1] + m_center[1], Z[i][j+1] + m_center[2], 1.0f), //vertex
          glm::vec4(glm::normalize(glm::vec3(X[i][j+1], Y[i][j+1], Z[i][j+1])), 0.0f))); //normals


        }
     }
        m_verticesSize = m_sphereAttributes.size();
        m_vbo.updateBufferData(m_sphereAttributes.data(), m_sphereAttributes.size()*sizeof(sphereAttributes));
        _check_gl_error(__FILE__,__LINE__);

}

void Sphere::render()
{
    if (!m_initialized)
    {
        std::cerr << "Sphere.render(): not rendering because not initialized yet. Use method initialize()" << std::endl;
        return;
    }
    m_program.useProgram();
    m_vao.bind();

    glEnable(GL_CULL_FACE);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, m_verticesSize);
//    glDrawArrays(GL_LINES, 0, m_verticesSize);

    glDisable(GL_CULL_FACE);

    m_program.release();
    m_vao.unbind();

}


void Sphere::setLighting()
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
    float shading_model       = 0.0f;
    //Light source Parameters
    glm::vec4 light_location      = glm::vec4(-1.0f,0.0,.0,1.0);
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
    float     shineness           = 25.0f;
    glm::vec4 expectator_position = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

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



int Sphere::getThetaRes()
{
    return m_thetaRes;
}

int Sphere::getPhiRes()
{
    return m_phiRes;
}

int Sphere::getSphereSize()
{
    return m_sphereSize;
}

int Sphere::getSpherePoints()
{
    return m_spherePoints;
}

void Sphere::_check_gl_error(const char *file, int line) {
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


//void sphere::setTextureCoords()
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
