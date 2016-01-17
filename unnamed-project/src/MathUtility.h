#ifndef UNNAMED_PROJECT_MATH_UTILITY_H
#define UNNAMED_PROJECT_MATH_UTILITY_H

#include <array>
#include <QVector2D>
#include <QMatrix4x4>

#define EPSILON 10e-10

namespace MathUtility
{
    /**
     * @brief rotatingCalipers Algorithm for creating a minimal
     *        bounding rectangle
     * @param points points to fit in the rectangle, have to be
     *        a sorted convex hull!
     * @param lowerPoint saves the lower point of the axis aligned rectangle
     * @param upperPoint saves the upper point of the axis aligned rectangle
     * @param rotation you have to rotate the box given by 'lowerPoint'
     *        and 'upperPoint' with this value to get the
     *        minimal bounding box
     */
    void getMinimalBoundingBox(
            const std::vector<QVector2D> &points,
            QVector2D &lowerPoint,
            QVector2D &upperPoint,
            float &rotation);

    /**
     * @brief getConvexHull get a convex Hull for a given point set
     * @param points    the given point set
     * @param hull      output vector for the hull
     */
    void getConvexHull(const std::vector<QVector2D> &points,
                       std::vector<QVector2D> &hull);

    inline bool isInEps(float a, float b)
    {
        return a > b ? a -b < EPSILON : b - a < EPSILON;
    }

    inline bool isInEps(const QVector2D &a, const QVector2D &b)
    {
        return isInEps(a[0], b[0]) && isInEps(a[1], b[1]);
    }

    template <typename Container>
    void transformVectors(const QMatrix4x4 &transformation, Container &corners)
    {
        for (auto &corner : corners)
        {
            QVector4D res = transformation * QVector4D(corner, 1.);
            corner = QVector3D(res / res.w());
        }
    }
}


#endif // UNNAMED_PROJECT_MATH_UTILITY_H
