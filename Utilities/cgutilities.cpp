#include "cgutilities.h"

CGUtilities::CGUtilities()
{

}

void CGUtilities::_check_gl_error(const char *file, int line)
{
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

glm::mat4 CGUtilities::shiftYtoAxisMatrix(glm::vec3 axis)
{
    glm::vec3 normAxis = glm::normalize(axis);

    float gamma = -asin(normAxis.x);
    float beta = (cos(gamma) > 0.0000001) ? acos(normAxis.y/cos(gamma)) : 0.0f;

    glm::mat4 xRot = glm::rotate(beta, glm::vec3(1.0,0.0,0.0));
    glm::mat4 zRot = glm::rotate(gamma, glm::vec3(0.0,0.0,1.0));
    glm::mat4 tMat = zRot*xRot;

    return (tMat);
}

