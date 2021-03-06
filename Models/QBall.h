#ifndef QBALL_H
#define QBALL_H

//#include "../../../file_reader/dicom/vmtkDiffAcquisition.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <math.h>
#include "SphericalHarmonics.h"
#include "complex.h"
//#include <boost/math/special_functions/spherical_harmonic.hpp>


#include "../Utilities/CGUtilities.h"
/**
 * @ingroup DWI_Volume_Processing
 *
 * @brief Compute and stores the ODF for each voxel of a DWI based on the QBall Imaging.
 * The algorithm and variable names are based on the article Q-Ball Imaging, by David S. Tuch (2004)
 *
 *Takes the vmtkDiffDiffAcquisition class as input to compute, as well as number of elements on phi and
 *theta for a spherical mesh, the amount of equator points for each direction and the kernel used.
 *
 * Temporary variables have their same name as the article.
 *
 */

class vmtkDiffAcquisition;

class QBall: protected CGUtilities
{
  public:
enum KERNEL{
        GAUSSIAN,
        INVERSE_MULTIQUADRIC,
        THIN_PLATE_SPLINE
    };
protected:
    int m_instancesAmount; /**< Maximum amount of objects to be drawn*/


    float **m_ODF; /**< Pointer that stores pointers to each ODF for each index */
    float *m_GFA;/**< array for all GFA computed for each voxel*/

    std::vector<glm::vec3> m_baseDirections;
    std::vector<unsigned int> m_indexBuffer;

    glm::mat4 * m_translateMatrix;
    float m_scale;
    int m_ODFsize;
    int m_SHorder;
public:

    QBall(int instancesAmount = 30, int icoIterations = 3, int thetaRes = 15, int phiRes = 15);
    virtual ~QBall();

    /**
    * @brief Initialization and computation of QBall Algorithm. All the intermediate variable names used has the same name as in the base article
    */
    virtual void computeODFs();
    void computeTranslationMatrices(const unsigned int& instancesAmount);

    /**
    * @brief returns the ODF for voxel indexed by index
    * @param index of the voxel on volume that QBall is computed
    */
    float *getVoxelODF(int index);

    /**
    * @brief returns the GFA for voxel indexed by index
    * @param index of the voxel on the volume that Qball is computed
    */
    float getVoxelGFA(int index);

    /**
    * @brief returns the amount of elements of the spherical mesh, represented by m_baseDirections
    */
    int getODFSize() const;

    /**
    * @brief returns a copy of the indexBuffer
    */
    std::vector<unsigned int> getIndexBuffer() const;

    /**
    * @brief returns a copy of the spherical mesh directions
    */
    std::vector<glm::vec3> getBaseDirections() const; /**< returns a copy of the spherical mesh directions*/


    int* getVolDimension() const;

    int getVoxelAmount() const;
    glm::mat4 getVoxelDisplacement(int index);

    int getInstancesAmount() const;
    float getScale() const;

protected:
    /**
    * @brief Apply the kernel already set up for two vectors, selected by m_kernel
    */
    inline float kernel(const glm::vec3& v1, const glm::vec3& v2);

    void generateIcoCoordsMeshSphere(const unsigned int meshIterations);
    void generateUVCoordsMeshSphere(const unsigned int thetaRes, const unsigned int phiRes);


//    SphericalHarmonics m_SHCoeff;

};
#endif // QBALL_H
