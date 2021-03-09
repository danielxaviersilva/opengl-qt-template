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


float QBall::getScale() const
{
    return m_scale;
}

QBall::QBall(int instancesAmount, int icoIterations, int thetaRes, int phiRes):
    m_instancesAmount(instancesAmount)
{

    bool isIco = true;

      if(isIco) {
        generateIcoCoordsMeshSphere(icoIterations);
      } else {
          generateUVCoordsMeshSphere(thetaRes, phiRes);
      }

      shrinkVec3(m_baseDirections, m_indexBuffer, true);
      computeTranslationMatrices(m_instancesAmount);
//      AlignEvenMesh(m_baseDirections, m_indexBuffer);
}

QBall::~QBall()
{
    for (int i = 0; i < m_instancesAmount; i++)
        delete[] m_ODF[i];
    delete[] m_ODF;

//    delete [] m_GFA;
    delete [] m_translateMatrix;
}

void QBall::computeODFs()
{

    m_ODF = new float*[m_instancesAmount];
    const std::vector<float>& currentODF = Psi679989;//Psi432324;
//    double *SH_out = new double[m_SHorder];

#pragma omp parallel for
    for (int idx = 0; idx < m_instancesAmount; idx++)
    {
        float * psi = new float[m_baseDirections.size()];
        for (size_t i = 0; i < m_baseDirections.size(); i++) {

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

            psi[i] = ((float)rand()/(float)(RAND_MAX));
//            if (psi[i] < 0.7f) psi[i] = 0.7f;
//            auto max = max_element(std::begin(currentODF), std::end(currentODF));
//            auto min = min_element(std::begin(currentODF), std::end(currentODF));
//            psi[i] = (currentODF[i]-*min)/(*max - *min);
//            psi[i] = (currentODF[i])/(*max);
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



}

void QBall::computeTranslationMatrices(const unsigned int& instancesAmount)
{
    int cols = (int) sqrt(instancesAmount);
    m_translateMatrix = new glm::mat4[cols*cols];
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
        }

    m_scale = 1.0f/((float)cols);

}

float *QBall::getVoxelODF(int index)
{
    return m_ODF[index];
}

float QBall::getVoxelGFA(int index)
{
    return m_GFA[index];
}

std::vector<unsigned int> QBall::getIndexBuffer() const
{
    return m_indexBuffer;
}

std::vector<glm::vec3> QBall::getBaseDirections() const
{
    return m_baseDirections;
}

glm::mat4 QBall::getVoxelDisplacement(int index)
{
    return m_translateMatrix[index];
}

void QBall::generateIcoCoordsMeshSphere(const unsigned int meshIterations)
{
    {
        const float t = (1.0 + std::sqrt(5.0)) / 2.0;
        m_baseDirections.push_back(glm::normalize(glm::vec3(-1.0,  t, 0.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3( 1.0,  t, 0.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3(-1.0, -t, 0.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3( 1.0, -t, 0.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3( 0.0, -1.0,  t)));
        m_baseDirections.push_back(glm::normalize(glm::vec3( 0.0,  1.0,  t)));
        m_baseDirections.push_back(glm::normalize(glm::vec3( 0.0, -1.0, -t)));
        m_baseDirections.push_back(glm::normalize(glm::vec3( 0.0,  1.0, -t)));
        m_baseDirections.push_back(glm::normalize(glm::vec3(   t, 0.0, -1.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3(   t, 0.0,  1.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3(  -t, 0.0, -1.0)));
        m_baseDirections.push_back(glm::normalize(glm::vec3(  -t, 0.0,  1.0)));

        m_indexBuffer = {0,11,5,0,5,1,
                         0,1,7,0,7,10,
                         0,10,11,1,5,9,
                         5,11,4,11,10,2,
                         10,7,6,7,1,8,
                         3,9,4,3,4,2,
                         3,2,6,3,6,8,
                         3,8,9,4,9,5,
                         2,4,11,6,2,10,
                         8,6,7,9,8,1};

        unsigned int currentVertexIdx = m_baseDirections.size();
        std::cout << "idxBufferSize/3 (#triangles): " << m_indexBuffer.size()/3 << std::endl;
        std::cout << "vertexListSize: " << m_baseDirections.size() << std::endl << std::endl;
        for (unsigned int it = 0; it < meshIterations; it++){

                std::vector<unsigned int> idxAux;// = m_indexBuffer;
                for (uint32_t i = 0; i < m_indexBuffer.size()/3; ++i) {
                    const uint32_t f0 = m_indexBuffer[i * 3];
                    const uint32_t f1 = m_indexBuffer[i * 3 + 1];
                    const uint32_t f2 = m_indexBuffer[i * 3 + 2];

                    const glm::vec3 v0 = glm::vec3(m_baseDirections[f0]);
                    const glm::vec3 v1 = glm::vec3(m_baseDirections[f1]);
                    const glm::vec3 v2 = glm::vec3(m_baseDirections[f2]);


                    const glm::vec3 v01 = glm::normalize(0.5f*(v0 + v1));
                    const glm::vec3 v12 = glm::normalize(0.5f*(v1 + v2));
                    const glm::vec3 v20 = glm::normalize(0.5f*(v2 + v0));

                    m_baseDirections.push_back(v01);
                    m_baseDirections.push_back(v12);
                    m_baseDirections.push_back(v20);

                    idxAux.push_back(f0);
                    idxAux.push_back(currentVertexIdx);
                    idxAux.push_back(currentVertexIdx+2);

                    idxAux.push_back(currentVertexIdx);
                    idxAux.push_back(f1);
                    idxAux.push_back(currentVertexIdx+1);

                    idxAux.push_back(currentVertexIdx+1);
                    idxAux.push_back(f2);
                    idxAux.push_back(currentVertexIdx+2);

                    idxAux.push_back(currentVertexIdx);
                    idxAux.push_back(currentVertexIdx+1);
                    idxAux.push_back(currentVertexIdx+2);

                    currentVertexIdx+=3;
                }
                m_indexBuffer = idxAux;
            }

    }

}

void QBall::generateUVCoordsMeshSphere(const unsigned int thetaRes, const unsigned int phiRes)
{
    m_indexBuffer.reserve(6*thetaRes*(phiRes-1));

        for (unsigned int i = 0; i< phiRes; i++) {
            const float phi = M_PI/2 - M_PI*i/(phiRes-1);
            for (unsigned int j = 0; j< thetaRes; j++) {
                const float theta = 2*(M_PI)*j/(thetaRes);
                m_baseDirections.push_back(glm::vec3(
                                                    cos(phi)*cos(theta),
                                                     sin(phi),
                                                     cos(phi)*sin(theta)
                                                     ));


                if ( i < phiRes -1 ){
                      m_indexBuffer.emplace_back(i*thetaRes + j                   );
                      m_indexBuffer.emplace_back((i+1)*thetaRes + j               );
                      m_indexBuffer.emplace_back((i+1)*thetaRes + (j+1)%thetaRes);
                      m_indexBuffer.emplace_back(i*thetaRes + j                   );
                      m_indexBuffer.emplace_back((i+1)*thetaRes + (j+1)%thetaRes);
                      m_indexBuffer.emplace_back(i*thetaRes + (j+1)%thetaRes    );
                }
            }
        }
}



