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
     */
    void getMinimalBoundingBox(
            const std::vector<QVector2D> &points,
            QVector2D &lowerPoint,
            QVector2D &upperPoint,
            float &rotation);

    float getAngle(const QVector2D &a, const QVector2D &b);
}


#endif // UNNAMED_PROJECT_MATH_UTILITY_H
