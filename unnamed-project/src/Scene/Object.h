#ifndef UNNAMED_PROJECT_SCENE_OBJECT_H
#define UNNAMED_PROJECT_SCENE_OBJECT_H

#include <QMatrix4x4>

#include "Scene/ObjectBase.h"

class Model;

class Object : public ObjectBase
{
public:
    Object(Model *model);

    void setColor(const QVector3D &color);

    QVector3D &getColor();

    void setSpecularAmount(float specular);
    void setDiffuseAmount(float diffuse);
    void setAmbientAmount(float ambient);

    float getSpecularAmount();
    float getDiffuseAmount();
    float getAmbientAmount();

    virtual ObjectType getObjectType();

    Model *getModel();
    void setModel(Model *model);

private:
    // model not owned
    Model *m_model;

    QVector3D m_color;

    float m_specularAmount;
    float m_diffuseAmount;
    float m_ambientAmount;

    // TODO material information
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_H
