#ifndef UNNAMED_PROJECT_MATH_UTILITY_H
#define UNNAMED_PROJECT_MATH_UTILITY_H

#include <array>
#include <QVector2D>

namespace mathUtility
{
/**
     * @brief rotatingCalipers Algorithm for creating a minimal
     *        bounding rectangle
     * @param points points to fit in the rectangle, have to be
     *        a convex hull!
     * @param lowerPoint saves the lower point of the axis aligned rectangle
     * @param upperPoint saves the upper point of the axis aligned rectangle
     * @param rotation you have to rotate the box with this value to get the
     *        minimal bounding box
     */
    void getMinimalBoundingBox(
            const std::vector<QVector2D> &points,
            QVector2D &lowerPoint,
            QVector2D &upperPoint,
            float &rotation);

    float getAngle(const QVector2D &a, const QVector2D &b);
}


#endif // UNNAMED_PROJECT_MATH_UTILITY_H
