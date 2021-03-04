#ifndef QBALLHEMISPHERE_H
#define QBALLHEMISPHERE_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <math.h>

#include "QBall.h"


class QBallHemisphere : public QBall
{
private:
    std::vector<glm::vec3> m_baseHemisphere;

public:
    QBallHemisphere(int instancesAmount = 30, int icoIterations = 3, int thetaRes = 15, int phiRes = 15);
    void computeODFs();
};

#endif // QBALLHEMISPHERE_H
