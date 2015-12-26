#include "MathUtility.h"
#include <climits>
#include <cmath>
#include <cassert>
#include <QMatrix2x2>

//DEBUG
#include <iostream>

struct AngleComparator{
    inline bool operator()(const std::pair<QVector2D, float> &a,
              const std::pair<QVector2D, float> &b)
    {
        return a.second < b.second;
    }
};

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
        QVector2D dP = points[i +1 >= points.size() ? 0 : i + 1] - points[i];
        alphas.push_back(std::atan2(dP[1], dP[0]));
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

    rotation = bestAlpha;
    lowerPoint = bestLowerPoint;
    upperPoint = bestUpperPoint;

}

//------------------------------------------------------------------------------
// helper Funktion:
inline float T(const QVector2D &a, const QVector2D &b, const QVector2D &c)
{
    return (b[0] - a[0])*(c[1] - a[1]) - (c[0] - a[0])*(b[1] - a[1]);
}

void mathUtility::getConvexHull(const std::vector<QVector2D> &points,
                                std::vector<QVector2D> &hull)
{
    assert(points.size() > 2);

    // Graham scan implementation
    hull.clear();

    float yMin = std::numeric_limits<float>::max();
    float y2ndMin = yMin;

    QVector2D minPoint;
    std::vector<std::pair<QVector2D, float>> listWithAngles;

    // get lowest point on y-axis.
    // if there are more than one,
    // take the one with minimal x-coordinate

    for (auto &p : points)
    {
        if (p[1] < yMin)
        {
            y2ndMin = yMin;
            yMin = p[1];
            minPoint = p;
        }
    }

    if (y2ndMin < yMin + EPSILON)
    {
        float xMin = minPoint[0];
        for (auto &p : points)
        {
            if (p[1] < yMin + EPSILON && p[0] < xMin)
            {
                xMin = p[0];
                minPoint = p;
            }
        }
    }

    // calculate angles for comparators
    for (auto &p : points)
    {
        if (!isInEps(p,minPoint))
        {
            float alpha = std::atan2(p[1] - minPoint[1],
                                     p[0] - minPoint[0]);
            listWithAngles.push_back(std::make_pair(p, alpha));
        }
    }

    typedef std::vector<std::pair<QVector2D, float>>::iterator
            angleListIterator;

    // sort listWithAngles
    std::sort<angleListIterator, AngleComparator>(listWithAngles.begin(),
                                                  listWithAngles.end(),
                                                  AngleComparator());

    if (listWithAngles.size() == 0)
    {
        // all points are in the same epsilon environment
        hull.push_back(minPoint);
        return;
    }

    // graham-scan:
    hull.push_back(minPoint);
    for (auto &pair : listWithAngles)
    {
        auto &pk = pair.first;
        std::cout << "Processing point "
                  << pk[0] << " -- " << pk[1]
                  << std::endl;
        while (hull.size() > 1)
        {
            size_t hSize = hull.size();
            float t = T(hull[hSize - 2], hull[hSize - 1], pk);

            if (t > 0) // pk is on the left side of p[hSize-2] → p[hSize-1]
            {
                break;
            }
            else
            {
                std::cout << "\tremove point : "
                          << hull[hSize-1][0]
                          << " -- "
                          << hull[hSize-1][1]
                          << std::endl;
                hull.pop_back();
            }
        }
        hull.push_back(pk);
    }

}

//------------------------------------------------------------------------------

