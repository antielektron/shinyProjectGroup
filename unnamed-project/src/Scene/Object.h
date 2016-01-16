#ifndef UNNAMED_PROJECT_SCENE_OBJECT_H
#define UNNAMED_PROJECT_SCENE_OBJECT_H

#include <QMatrix4x4>

#include "Scene/ObjectBase.h"

class Model;

class Object : public ObjectBase
{
public:
    Object(Model *model);

    void setSpecularColor(const QVector3D &specular);
    void setDiffuseColor(const QVector3D &diffuse);
    void setAmbientColor(const QVector3D &ambient);

    QVector3D &getSpecularColor();
    QVector3D &getDiffuseColor();
    QVector3D &getAmbientColor();

    virtual ObjectType getObjectType();

    Model *getModel();
    void setModel(Model *model);

private:
    // model not owned
    Model *m_model;

    QVector3D m_specularColor;
    QVector3D m_diffuseColor;
    QVector3D m_ambientColor;

    // TODO material information
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_H
