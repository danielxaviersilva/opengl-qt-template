#include "sphere.h"
#include "sphere.h"


sphere::sphere(int theta, int phi, float R, float xc, float yc, float zc)
{
    thetaRes = theta;
    phiRes = phi;
    radius = R;

    center[0] = xc;
    center[1] = yc;
    center[2] = zc;

    setSphereSurface();

    //setTextureCoords();

    bindSphere();
}

float sphere::getRadius() const
{
    return radius;
}


void sphere::bindSphere()
{
    glGenBuffers(1,&surface_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, surface_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*surface.size(),surface.data(),GL_STATIC_DRAW);


    glGenBuffers(1,&normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*normals.size(),normals.data(),GL_STATIC_DRAW);

    //Future: Do that to texture coords

}

void sphere::setSphereSurface()
{
    float * phi;
    float * theta;

    phi = new float[phiRes];
    theta = new float [thetaRes];

    float ** X;
    float ** Y;
    float ** Z;

    X = new float*[phiRes];
    Y = new float *[phiRes];
    Z = new float *[phiRes];

    for (int count = 0; count < phiRes; count++)
    {
        X[count] = new float [thetaRes];
        Y[count] = new float [thetaRes];
        Z[count] = new float [thetaRes];
    }

    for (int i = 0; i < thetaRes; i++)
        theta[i] = 2*(  M_PI)*i/(thetaRes - 1);

    for (int i = 0; i < phiRes; i++)
        phi[i] = M_PI/2 - M_PI*i/(phiRes - 1);

    for (int i = 0; i< phiRes; i++)
    {
        for (int j = 0; j< thetaRes; j++)
        {
            X[i][j] = radius*(cos(phi[i])*cos(theta[j]));
            Y[i][j] = radius*(cos(phi[i])*sin(theta[j]));
            Z[i][j] = radius*(sin(phi[i]));
        }
    }

    int idx = 0;
    for (int i = 0; i < phiRes-1; i++)
            {
                for(int j = 0; j<thetaRes -1; j++)
                {
                    surface.push_back(glm::vec4(X[i][j] + center[0], Y[i][j] + center[1], Z[i][j] + center[2], 1.0f));
                    surface.push_back(glm::vec4(X[i+1][j] + center[0], Y[i+1][j] + center[1], Z[i+1][j] + center[2], 1.0f));
                    surface.push_back(glm::vec4(X[i+1][j+1] + center[0], Y[i+1][j+1] + center[1], Z[i+1][j+1] + center[2], 1.0f));

                    surface.push_back(glm::vec4(X[i+1][j+1] + center[0], Y[i+1][j+1] + center[1], Z[i+1][j+1] + center[2], 1.0f));
                    surface.push_back(glm::vec4(X[i][j+1] + center[0], Y[i][j+1] + center[1], Z[i][j+1] + center[2], 1.0f));
                    surface.push_back(glm::vec4(X[i][j] + center[0], Y[i][j] + center[1], Z[i][j] + center[2], 1.0f));

                    //idx++;
                }
            }

}

void sphere::setSphereNormal()
{
    for (int i = 0; i < int(surface.size()); i++)
        normals.push_back(glm::vec4(glm::normalize(glm::vec3(surface[i]) - glm::make_vec3(center)), 0.0f));
}



int sphere::getThetaRes()
{
    return thetaRes;
}

int sphere::getPhiRes()
{
    return phiRes;
}

int sphere::getSphereSize()
{
    return sphereSize;
}

int sphere::getSpherePoints()
{
    return spherePoints;
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
