#ifndef UNNAMED_PROJECT_OBJECT_H
#define UNNAMED_PROJECT_OBJECT_H

#include <QMatrix4x4>

class Model;

class Object
{
public:
    Object(Model *model);

    void setWorld(const QMatrix4x4 &world);

    void setAlpha(float a);
    void setSpecularAmount(float spec);
    void setDiffuseAmount(float diff);

    float getAlpha();
    float getSpecularAmount();
    float getDiffuseAmount();

    QMatrix4x4 &getWorld();

    Model *getModel();

private:
    // model not owned
    Model *m_model;
    QMatrix4x4 m_world;

    //material information for phong shading:
    float alpha;
    float specularAmount;
    float diffuseAmount;


    // TODO material information
};

#endif // UNNAMED_PROJECT_OBJECT_H
