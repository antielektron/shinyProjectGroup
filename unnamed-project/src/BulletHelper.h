#ifndef UNNAMED_PROJECT_BULLET_HELPER_H
#define UNNAMED_PROJECT_BULLET_HELPER_H

#ifdef HAVE_BULLET

#include <QVector3D>
#include <btBulletDynamicsCommon.h>

inline QVector3D toQVector3D(const btVector3 &input)
{
    return QVector3D(input.x(), input.y(), input.z());
}

inline btVector3 toBulletVector3(const QVector3D &input)
{
    return btVector3(input.x(), input.y(), input.z());
}

#endif // HAVE_BULLET

#endif // UNNAMED_PROJECT_BULLET_HELPER_H
