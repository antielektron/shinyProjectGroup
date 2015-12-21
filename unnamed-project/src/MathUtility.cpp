#include "MathUtility.h"
#include <climits>
#include <QMatrix2x2>

void mathUtility::getMinimalBoundingBox(
        const std::vector<QVector2D> &points,
        QVector2D &lowerPoint,
        QVector2D &upperPoint,
        float &rotation)
{
    // see also https://geidav.wordpress.com/2014/01/23/computing-oriented-minimum-bounding-boxes-in-2d/

    float a_min = std::numeric_limits<float>::max();

    QVector2D bestLowerPoint;
    QVector2D bestUpperPoint;
    float bestAlpha = 0;

    std::vector<float> alphas;
    std::vector<QVector2D> transformedPoints;
    transformedPoints.assign(points.size(), QVector2D(0,0));

    for (size_t i = 0; i < points.size(); i++)
    {
        QVector2D dP = points[i +1 >= points.size() ? 0 : i] - points[i];
        alphas.push_back(getAngle(QVector2D(0.f,0.f), dP));
    }

    for (float alpha : alphas)
    {
        // create rotation Matrix:
        float ca = std::cos(-alpha);
        float sa = std::sin(-alpha);


        // rotate points:
        for (size_t i = 0; i < points.size(); i++)
        {
            // hardcoded rotation:
            float newX = ca * points[i][0] - sa * points[i][1];
            float newY = sa * points[i][0] + ca * points[i][1];
            transformedPoints[i][0] = newX;
            transformedPoints[i][1] = newY;
        }

        // test for min rectangle:
        float xMax = -std::numeric_limits<float>::max();
        float yMax = -std::numeric_limits<float>::max();
        float xMin = std::numeric_limits<float>::max();
        float yMin = std::numeric_limits<float>::max();
        float area;

        for (auto p : transformedPoints)
        {
            if (p[0] > xMax)
            {
                xMax = p[0];
            }
            if (p[0] < xMin)
            {
                xMin = p[0];
            }
            if (p[1] > yMax)
            {
                yMax = p[1];
            }
            if (p[1] < yMin)
            {
                yMin = p[1];
            }
        }
        // calculate area:
        area = (xMax - xMin) * (yMax - yMin);
        if (area < a_min)
        {
            a_min = area;
            bestAlpha = alpha;
            bestLowerPoint = QVector2D(xMin, yMin);
            bestUpperPoint = QVector2D(xMax, yMax);
        }

    }

    rotation = -bestAlpha;
    lowerPoint = bestLowerPoint;
    upperPoint = bestUpperPoint;

}

//------------------------------------------------------------------------------
float mathUtility::getAngle(const QVector2D &a, const QVector2D &b)
{
    float sinTheta = std::fabs(a[0] * b[1] - a[1] * b[0])/(a.length() * b.length());
    return std::asin(sinTheta);
}

//------------------------------------------------------------------------------

