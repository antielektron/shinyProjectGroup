#ifndef UNNAMED_PROJECT_SCENE_SCENE_H
#define UNNAMED_PROJECT_SCENE_SCENE_H

#include <QMatrix4x4>
#include <QColor>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <utility>

//for import-/export functions
#include <QtXml/QDomDocument>
#include <QFile>
#include <QXmlStreamWriter>

#include "Scene/Model.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"
#include "smartiterator.h"

#include "SceneEditor/EditorObject.h"
#include "GameLogic/GlobalState.h"
#include "GameLogic/Animations/AnimationBase.h"
#include "IObjectBaseObserver.h"

class Scene
{
public:
    Scene();
    Scene(const QString &filename);

    void loadFromFile(const QString &filename);
    void saveToFile(const QString &filename);

    void setCameraView(const QMatrix4x4 &camera);
    void setCameraProjection(const QMatrix4x4 &proj);

    void setLightColor(const QVector3D &color);
    void setDirectionalLightDirection(const QVector3D &direction);

    QMatrix4x4 &getCameraView();
    QMatrix4x4 &getCameraProjection();

    QVector3D &getDirectionalLightDirection();
    QVector3D &getLightColor();

    // Models
    void addModel(std::unique_ptr<Model> model);
    void removeModel(const std::string &modelName);

    Model *getModel(const std::string &modelName);

    typedef std::map<std::string, std::unique_ptr<Model>>::const_iterator ModelIterator;
    range<ModelIterator> getModels();


    // Objects
    Object *createObject(const std::string &modelName, ObjectGroup *parent = nullptr);
    ObjectGroup *createObjectGroup(const std::string &name, ObjectGroup *parent = nullptr);

    EditorObject *createEditorObject(const std::string &name, Model *modelName);

    ObjectGroup *getSceneRoot();

    void updateObjectList();

    typedef std::vector<Object *>::const_iterator ObjectIterator;
    range<ObjectIterator> getObjects();

    range<ObjectGroup::object_iterator_type> getEditorObjects();


    // Animations
    void addAnimation(std::unique_ptr<AnimationBase> animation);
    void deleteAnimation(AnimationBase *animation);

    void performAnimations(float time, IObjectBaseObserver *listener = nullptr);
    void cancelAllAnimations();

    typedef std::vector<std::unique_ptr<AnimationBase>>::const_iterator AnimationIterator;
    range<AnimationIterator> getAnimations();


    GlobalState *getGlobalState();


    // Access metadata
    void setName(const QString &name);
    void setVersion(const QString &version);
    void setAuthor(const QString &author);

    const QString &getName() const;
    const QString &getVersion() const;
    const QString &getAuthor() const;

    ObjectBase *findObjectByName(ObjectGroup *root, const QString &name);

private:
    void readObjectTreeFromDom(ObjectGroup *root, const QDomElement &domElement);
    void readModelsFromDom(const QDomElement &domElem);
    void readEventsFromDom(const QDomElement &domElem);
    void readAttributesFromDom(const QDomElement &domElem);
    void readAnimatorsFromDom(const QDomElement &domElem);

    QVector3D readVectorFromDom(const QDomElement &domElement, const QVector3D &defValue = QVector3D(0, 0, 0));

    void writeObjectTree(ObjectGroup *root, QXmlStreamWriter &writer);
    void writeModels(QXmlStreamWriter &writer);
    void writeAttribute(QXmlStreamWriter &writer, const QString &key, const QVariant &value);
    void writeVectorToXml(const QVector3D &position, QXmlStreamWriter &writer);
    void writeRotation(const QVector3D &rotation, QXmlStreamWriter &writer);
    void writeScaling(const QVector3D &scaling, QXmlStreamWriter &writer);

    void addToObjectList(ObjectGroup *group);

    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    /// QMatrix4x4 m_world;

    QVector3D m_directionalLightDirection;
    QVector3D m_lightColor;

    // No pointer: force to exist.
    ObjectGroup m_rootGroup;

    // Root for Editor- (aka Debug-) objects.
    // Scene only allows to add objects to that root,
    // groups are not allowed as child
    ObjectGroup m_editorObjectRootGroup;

    //TODO: get names directly from objects
    //(is more efficient than ask the scene
    // every time for it! )
    // Model <- SceneModel <- SceneObject hm
    std::map<std::string, std::unique_ptr<Model>> m_models; // all basic models that are available (only for construction purposes, if needed!)

    std::vector<Object *> m_objects;

    std::vector<std::unique_ptr<AnimationBase>> m_animations;

    // Meta information
    QString m_sceneName;
    QString m_sceneVersion;
    QString m_sceneAuthor;

    std::unique_ptr<GlobalState> m_globalState;
};

#endif // UNNAMED_PROJECT_SCENE_SCENE_H
