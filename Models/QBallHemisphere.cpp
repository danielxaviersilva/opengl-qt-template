#include "QBallHemisphere.h"
#include <chrono>
#include <glm/gtx/string_cast.hpp>
#include <fstream>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <omp.h>
#include<iostream>
#include<QBallGlyphsCoefficientsSample.h>

QBallHemisphere::QBallHemisphere(int instancesAmount, int icoIterations, int thetaRes, int phiRes):
    QBall( instancesAmount, icoIterations, thetaRes, phiRes)
{
    AlignEvenMesh(m_baseDirections, m_indexBuffer);

    for (size_t i = 0; i < m_baseDirections.size(); i+=2) {
        m_baseHemisphere.push_back(m_baseDirections[i]);
    }

}

void QBallHemisphere::computeODFs()
{
    m_ODF = new float*[m_instancesAmount];
    const std::vector<float>& currentODF = Psi679989;//Psi432324;
//    double *SH_out = new double[m_SHorder];

#pragma omp parallel for
    for (int idx = 0; idx < m_instancesAmount; idx++)
    {
        float * psi = new float[m_baseHemisphere.size()];
        for (size_t i = 0; i < m_baseHemisphere.size(); i++) {

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
//            float SH = /*std::sqrtf(5/(16*M_PI))**/(3*m_baseHemisphere[i].z*m_baseHemisphere[i].z - 1)/2;
//            psi[i] = abs(SH);
//            std::cout << SH << std::endl;
//            if(isnan(SH_out[SHidx])) {
//                std::cout << "PHI NAN: " << asin(m_sphereAttributesBuffer[i].y) << std::endl;
//            }


            psi[i] = ((float)rand()/(float)(RAND_MAX));
            if (psi[i] < 0.7f) psi[i] = 0.7f;
//            auto max = max_element(std::begin(currentODF), std::end(currentODF));
//            auto min = min_element(std::begin(currentODF), std::end(currentODF));
//            psi[i] = (currentODF[i]-*min)/(*max - *min);
//            psi[i] = (currentODF[i])/(*max);

        }
        m_ODF[idx] = psi;
    }
}
