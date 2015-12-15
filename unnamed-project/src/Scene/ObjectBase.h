#ifndef UNNAMED_PROJECT_SCENE_OBJECT_BASE_H
#define UNNAMED_PROJECT_SCENE_OBJECT_BASE_H

#include <QVector3D>
#include <QString>
#include <QMatrix4x4>

class ObjectGroup;

// not sure if this is the smartest solution (Yes)
enum class ObjectType
{
    Undefined,
    ObjectGroup,
    Object
};

class ObjectBase
{
public:
    ObjectBase(ObjectGroup *parent = nullptr);

    // void setWorld(const QMatrix4x4 &world);
    virtual void updateWorld();

    void setPosition(const QVector3D &position);
    QVector3D &getPosition();

    void setRotation(const QVector3D &rotation);
    QVector3D &getRotation();

    void setScaling(const QVector3D &scaling);
    QVector3D &getScaling();

    const QMatrix4x4 &getWorld();

    void setParent(ObjectGroup *parent);
    ObjectGroup *getParent();

    void setName(QString name);
    QString getName();

    // destroys itself
    void destroy();

    virtual ObjectType getObjectType();

protected:
    ObjectGroup *m_parent;

    QVector3D m_position;
    QVector3D m_scaling;
    QVector3D m_rotation;

    QMatrix4x4 m_world;

    QString m_name;
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_BASE_H
