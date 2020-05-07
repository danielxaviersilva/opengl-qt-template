#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <vector>
#include <math.h>

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
public:
   // sphere();
    sphere(int theta = 30, int phi = 30, float r = 0.5, float xc = 0, float yc = 0, float zc = 0);

    vector <float> surface;
    vector <float> normals;
    vector <float> textureCoords;


    void setSphereSurface();
    void setSphereNormal();
    void setTextureCoords();

    int getThetaRes();
    int getPhiRes();

    int getSphereSize();
    int getSpherePoints();


    float getRadius() const;

    void normalize(float *v);
};

#endif // SPHERE_H
