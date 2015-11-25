#ifndef UNNAMED_PROJECT_SCENE_OBJECT_H
#define UNNAMED_PROJECT_SCENE_OBJECT_H

#include <QMatrix4x4>

#include "Scene/ObjectBase.h"

class Model;

class Object : public ObjectBase
{
public:
    Object(Model *model);

    void setAlpha(float a);
    void setSpecularAmount(float spec);
    void setDiffuseAmount(float diff);

    float getAlpha();
    float getSpecularAmount();
    float getDiffuseAmount();

    Model *getModel();

private:
    // model not owned
    Model *m_model;

    //material information for phong shading:
    float alpha;
    float specularAmount;
    float diffuseAmount;

    // TODO material information
};

#endif // UNNAMED_PROJECT_SCENE_OBJECT_H
