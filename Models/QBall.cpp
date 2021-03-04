#include "QBall.h"
#include <chrono>
#include <glm/gtx/string_cast.hpp>
#include <fstream>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <omp.h>
#include<iostream>
#include<QBallGlyphsCoefficientsSample.h>


using namespace std::chrono;

#define CUBE_EDGE sqrt(3)/3

float cube2sphere(float x, float y, float z)
{
    if ( (x*x >= y*y) && (x*x >= z*z) )
       return glm::length(glm::vec3(CUBE_EDGE, y/abs(x), z/abs(x)));
    if( (y*y >= z*z) && (y*y >= x*x) )
        return glm::length(glm::vec3(CUBE_EDGE, x/abs(y), z/abs(y)));

    return glm::length(glm::vec3(CUBE_EDGE, x/abs(z), y/abs(z)));
}



int QBall::getVoxelAmount() const
{
    return m_instancesAmount;
}

int QBall::getInstancesAmount() const
{
    return m_instancesAmount;
}

void QBall::setInstancesAmount(int instancesAmount)
{
    m_instancesAmount = instancesAmount;
}

float QBall::getScale() const
{
    return m_scale;
}

QBall::QBall(int vertexRes, int instancesAmount, int thetaRes, int phiRes, int SH_order):
    m_vertexRes(vertexRes/*errado*/), m_instancesAmount(instancesAmount), m_SHorder(SH_order), m_SHCoeff(SH_order)
{
    m_instancesAmount = 2*m_SHorder + 1;
    int cols = (int) sqrt(m_instancesAmount) + 1;
    m_translateMatrix = new glm::mat4[cols*cols];
    m_ODF = new float*[m_instancesAmount];


    setSphereSurface(thetaRes, phiRes);

    m_vertexRes = m_sphereAttributesBuffer.size();
    const std::vector<float>& currentODF = Psi679989;//Psi432324;
    double *SH_out = new double[m_SHorder];

#pragma omp parallel for
    for (int idx = 0; idx < m_instancesAmount; idx++)
    {
        float * psi = new float[m_vertexRes];
        for (int i = 0; i < m_vertexRes; i++) {

//            const double phi = asin(m_sphereAttributesBuffer[i].y);
////            if(isnan(phi)) {
////                std::cout << "PHI NAN: " << m_sphereAttributesBuffer[i].y) << std::endl;
////            }
//            double theta;
//            if(cos(phi) > 0.00001)
//                theta = acos(m_sphereAttributesBuffer[i].x/cos(phi));
//            else
//                theta = 0.0;

//            auto SH = std::abs(boost::math::spherical_harmonic(/*m_SHorder*/m_SHorder, idx - m_SHorder , m_phiSet[i] , m_thetaSet[i] ));
//            SH = SH*SH;
//            float SH = std::sqrtf(5/(16*M_PI))*(3*m_sphereAttributesBuffer[i].z*m_sphereAttributesBuffer[i].z - 1);

//            std::cout << SH << std::endl;
//            if(isnan(SH_out[SHidx])) {
//                std::cout << "PHI NAN: " << asin(m_sphereAttributesBuffer[i].y) << std::endl;
//            }
//            psi[i] = abs(SH);

//            psi[i] = ((float)rand()/(float)(RAND_MAX));
//            if (psi[i] < 0.7f) psi[i] = 0.7f;
            auto max = max_element(std::begin(currentODF), std::end(currentODF));
            auto min = min_element(std::begin(currentODF), std::end(currentODF));
            psi[i] = (currentODF[i]-*min)/(*max - *min);
            psi[i] = (currentODF[i])/(*max);
//            psi[i] = Psi679871[i];
//            psi[i] = (std::min({fabs(m_sphereAttributesBuffer[i].x),
//                                fabs(m_sphereAttributesBuffer[i].y),
//                                fabs(m_sphereAttributesBuffer[i].z)}));




//            psi[i] = cube2sphere(m_sphereAttributesBuffer[i].x,
//                                 m_sphereAttributesBuffer[i].y,
//                                 m_sphereAttributesBuffer[i].z);
        }

        m_ODF[idx] = psi;
    }



    std::vector<float> borderMeshX;
    std::vector<float> borderMeshY;
    std::vector<glm::vec2> centerSet;

    for(float i = -1; i <= 1.01; i+=(2.f/float(cols)))
        borderMeshX.push_back(i);

    for(float j = 1; j >= -1.01; j-=2.f/float(cols))
        borderMeshY.push_back(j);



    int idx = 0;
    for(int j = 0; j < cols; j++)
        for(int i = 0; i < cols; i++){
            m_translateMatrix[idx++] = glm::translate(glm::mat4(1.0f),
                                                      glm::vec3(0.5f*(borderMeshX[i] + borderMeshX[i+1]),
                                                                0.5f*(borderMeshY[j] + borderMeshY[j+1]), 0.0f));

//        std::cout << idx-1 << ": " << glm::to_string(m_translateMatrix[idx-1]) << std::endl;
        }

    m_scale = 1.0f/((float)cols);
}


QBall::~QBall()
{
    for (int i = 0; i < m_instancesAmount; i++)
        delete[] m_ODF[i];
    delete[] m_ODF;

    delete [] m_GFA;
    delete [] m_translateMatrix;
}

float *QBall::getVoxelODF(int index)
{
    return m_ODF[index];
}

float QBall::getVoxelGFA(int index)
{
    return m_GFA[index];
}

glm::mat4 QBall::getVoxelDisplacement(int index)
{
    return m_translateMatrix[index];
}


void QBall::setSphereSurface(int thetaRes, int phiRes)
{
//    std::vector <baseSphereAttributes> sphereAttributesBuffer;



    m_idxSet.reserve(6*thetaRes*(phiRes-1));
    m_sphereAttributesBuffer.reserve(phiRes*thetaRes);
    //Interval of meshes:
    //phi: [-pi/2, pi/2]
    //theta: [0, 2pi)
    //The loop is designed to make a closed circle on theta,
    //The only repetition of elements on buffer are when phi = ±pi/2

    for (int i = 0; i < phiRes; i++)
    {
        float phi =  M_PI/2 - M_PI*i/(phiRes - 1);
        for(int j = 0; j<thetaRes; j++)
        {
             float theta = 2*(M_PI)*j/(thetaRes);

//             sphereAttributesBuffer.emplace_back(baseSphereAttributes(
//             glm::vec4(cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta), 1.0f), //vertex
//             glm::vec4(cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta), 0.0f))); //normals

             m_sphereAttributesBuffer.emplace_back(glm::vec3
             (cos(phi)*cos(theta), sin(phi), cos(phi)*sin(theta))); //normals

             m_phiSet.push_back(phi);
             m_thetaSet.push_back(theta);

              if (i < phiRes -1)
              {
                    m_idxSet.emplace_back(i*thetaRes + j                   );
                    m_idxSet.emplace_back((i+1)*thetaRes + j               );
                    m_idxSet.emplace_back((i+1)*thetaRes + (j+1)%thetaRes);
                    m_idxSet.emplace_back(i*thetaRes + j                   );
                    m_idxSet.emplace_back((i+1)*thetaRes + (j+1)%thetaRes);
                    m_idxSet.emplace_back(i*thetaRes + (j+1)%thetaRes    );
              }
        }
     }

        shrinkVec3(m_sphereAttributesBuffer, m_idxSet, true);
        m_ODFsize = m_sphereAttributesBuffer.size();

//        m_verticesSize = idxSet.size();//sphereAttributesBuffer.size();

//        m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(baseSphereAttributes));
//          m_SphereVBO.updateBufferData(sphereAttributesBuffer.data(), sphereAttributesBuffer.size()*sizeof(glm::vec3));
//        _check_gl_error(__FILE__,__LINE__);


//        m_idxBuffer.updateBufferData(idxSet.data(),idxSet.size());
//        m_idxBuffer.unbind();
//        _check_gl_error(__FILE__,__LINE__);
}



