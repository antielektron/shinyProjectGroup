#ifndef UNNAMED_PROJECT_OBJECT_H
#define UNNAMED_PROJECT_OBJECT_H

#include <QMatrix4x4>

class Model;

class Object
{
public:
    Object(Model *model);

    void setWorld(const QMatrix4x4 &world);
    QMatrix4x4 &getWorld();

    Model *getModel();

private:
    // model not owned
    Model *m_model;
    QMatrix4x4 m_world;

    // TODO material information
};

#endif // UNNAMED_PROJECT_OBJECT_H
