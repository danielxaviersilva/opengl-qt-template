#ifndef CGUTILITIES_H
#define CGUTILITIES_H
#include <QGLFunctions>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtx/transform.hpp>


class CGUtilities
{
protected:
    CGUtilities();
    void _check_gl_error(const char *file, int line);
     glm::mat4 shiftYtoAxisMatrix(glm::vec3 axis);



};

#endif // CGUTILITIES_H
