#include "CGUtilities.h"

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

glm::mat4 CGUtilities::shiftZtoAxisMatrix(glm::vec3 axis)
{
    glm::vec3 normAxis = glm::normalize(axis);
    float beta = asin(normAxis.y);
    float gamma = (abs(cos(beta)) > 0.0000001) ? acos(normAxis.x/cos(beta)) : 0.0f;

    glm::mat4 xRot = glm::rotate(beta, glm::vec3(1.0,0.0,0.0));
    glm::mat4 yRot = glm::rotate(gamma, glm::vec3(0.0,1.0,0.0));
    glm::mat4 tMat = yRot*xRot;

    return tMat;
}

void CGUtilities::shrinkVec3(std::vector<glm::vec3> &V, std::vector<unsigned int> &index, bool verbose)
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
            if (glm::dot(glm::vec3(V[i]), glm::vec3(V[j])) == 1.0f)
                indexChanges[j] = i;

    for (int i = 0; i < vSize -1;i++)
        for (int j = i+1; j < vSize; j++)
            if (glm::dot(glm::vec3(V[i]), glm::vec3(V[j])) == 1.0f){
                V.erase(V.begin() + j);
                for (auto & id: indexChanges)//unsigned int k = 0; k < indexChanges.size(); k++)
                    if (id >= j)
                        id--;
                j--;
                vSize--;
            }

    for (unsigned int i = 0; i < index.size(); i++)
        index[i] = indexChanges[index[i]];

    if (verbose)
        std::cout << V.size() << std::endl;
}
