#ifndef UNNAMED_PROJECT_SCENE_OBJECT_BASE_H
#define UNNAMED_PROJECT_SCENE_OBJECT_BASE_H

#include <QMatrix4x4>

class ObjectBase
{
public:
    void setWorld(const QMatrix4x4 &world);

    QMatrix4x4 &getWorld();

protected:
    QMatrix4x4 m_world;
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_BASE_H
