#ifndef UNNAMED_PROJECT_SCENE_OBJECT_H
#define UNNAMED_PROJECT_SCENE_OBJECT_H

#include <QMatrix4x4>

#include "Scene/ObjectBase.h"

class Model;

class Object : public ObjectBase
{
public:
    Object(Model *model);

    void setSpecularAmount(float specular);
    void setDiffuseAmount(float diffuse);
    void setAmbientAmount(float ambient);

    float getSpecularAmount();
    float getDiffuseAmount();
    float getAmbientAmount();

    Model *getModel();

private:
    // model not owned
    Model *m_model;

    // TODO color
    float specularAmount;
    float diffuseAmount;
    float ambientAmount;

    // TODO material information
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_H
