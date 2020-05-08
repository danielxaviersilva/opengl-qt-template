#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <OpenGL.h>
#include <QGLFunctions>

//describes sphere that each coordinate is in vec4 form (x,y,z alfa=1).

using namespace std;

class sphere
{
    int thetaRes;
    int phiRes;
    int sphereSize;
    int spherePoints;
    float radius;
    float center[3];

    vector <glm::vec4> surface;
    vector <glm::vec4> normals;
    vector <glm::vec4> textureCoords;

    GLuint surface_vbo;
    GLuint normals_vbo;
public:
   // sphere();
    sphere(int theta = 30, int phi = 30, float r = 0.5, float xc = 0, float yc = 0, float zc = 0);

    void setSphereSurface();
    void setSphereNormal();
//    void setTextureCoords();

    int getThetaRes();
    int getPhiRes();
    int getSphereSize();
    int getSpherePoints();


    float getRadius() const;

    void uploadSphere();
private:
    void bindSphere();
};

#endif // SPHERE_H
