#ifndef UNNAMED_PROJECT_SCENE_GAME_OBJECT_H
#define UNNAMED_PROJECT_SCENE_GAME_OBJECT_H

#include <QVector3D>

class ObjectBase;

class GameObject
{
public:
    GameObject(ObjectBase *object);

    void setPosition(const QVector3D &position);

    void setRotation(const QVector3D &rotation);
    void setRotation(float yaw, float pitch, float roll);

    // template<int Index>
    // void setRotation(float r);

    ObjectBase *getObject();

    const QVector3D &getPosition() const;

    const QVector3D &getRotation() const;

    void applyTransformation();

private:
    // NOTE: not owned!
    ObjectBase *m_object;

    QVector3D m_position;
    QVector3D m_rotation; // yaw pitch roll
    // QVector3D m_scaling;
};

#endif // UNNAMED_PROJECT_SCENE_GAME_OBJECT_H
