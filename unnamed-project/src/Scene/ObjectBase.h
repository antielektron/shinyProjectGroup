#ifndef UNNAMED_PROJECT_SCENE_OBJECT_BASE_H
#define UNNAMED_PROJECT_SCENE_OBJECT_BASE_H

#include <QVector3D>
#include <QMatrix4x4>

class ObjectBase
{
public:
    ObjectBase();

    // void setWorld(const QMatrix4x4 &world);
    void updateWorld();

    void setPosition(const QVector3D &position);
    QVector3D &getPosition();

    void setRotation(const QVector3D &rotation);
    QVector3D &getRotation();

    void setScaling(const QVector3D &scaling);
    QVector3D &getScaling();

    const QMatrix4x4 &getWorld();

protected:
    QVector3D m_position;
    QVector3D m_scaling;
    QVector3D m_rotation;

    QMatrix4x4 m_world;
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_BASE_H
