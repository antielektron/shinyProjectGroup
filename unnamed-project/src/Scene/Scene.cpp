#include <cassert>
#include <iostream>
#include <stdexcept>

#include <QVector3D>

#include "Scene/Scene.h"
#include "GameLogic/Event.h"

//------------------------------------------------------------------------------
Scene::Scene()
{
    m_rootGroup.setName("SceneRoot");
    m_globalState.reset(new GlobalState(this));
}

//------------------------------------------------------------------------------
Scene::Scene(const QString &filename)
{
    m_globalState.reset(new GlobalState(this));
    m_rootGroup.setName("SceneRoot");
    loadFromFile(filename);
}

//------------------------------------------------------------------------------
void Scene::loadFromFile(const QString &filename)
{
    // Good example for reading xml files with QT:
    // http://www.lucidarme.me/?p=2534

    // Open file
    QDomDocument document;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Could not open file '" + filename.toStdString() + "'");
    }
    document.setContent(&file);
    file.close();

    // Parse header
    QDomElement root = document.documentElement();
    QString rootType = root.tagName();              // should be 'Scene'
    m_sceneName = root.attribute("name", "unnamedScene");
    m_sceneVersion = root.attribute("version", "0.0");
    m_sceneAuthor = root.attribute("author", "Norby");

    // DEBUG
    std::cout << "Scene Info: " << std::endl
              << "\tname: " << m_sceneName.toStdString() << std::endl
              << "\tversion: " << m_sceneVersion.toStdString() << std::endl
              << "\tauthor: " << m_sceneAuthor.toStdString() << std::endl
              << std::endl;

    // Parse xml body
    for (auto currentElement = root.firstChildElement(); !currentElement.isNull(); currentElement = currentElement.nextSiblingElement())
    {
        QString tag = currentElement.tagName();
        std::cout << "TAG " << tag.toStdString() << std::endl;
        if (tag == "Objects")
        {
            readObjectTreeFromDom(&m_rootGroup, currentElement);
        }
        else if (tag == "Models")
        {
            readModelsFromDom(currentElement);
        }
        else if (tag == "Events")
        {
            readEventsFromDom(currentElement);
        }
        else if (tag == "Attributes")
        {
            readAttributesFromDom(currentElement);
        }
        else if (tag == "Animators")
        {
            readAnimatorsFromDom(currentElement);
        }
        else if (tag == "DirectionalLight")
        {
            // TODO subelements
            QVector3D pos = readVectorFromDom(currentElement);
            float r = currentElement.attribute("r", "0").toFloat();
            float g = currentElement.attribute("g", "0").toFloat();
            float b = currentElement.attribute("b", "0").toFloat();

            this->setDirectionalLightDirection(pos);
            this->setLightColor(QVector3D(r,g,b));
        }
        else if (tag == "Player")
        {
            // TODO: embed "Spawn" objects into the object tree, that the "real" game can query for!
            QVector3D pos = readVectorFromDom(currentElement);

            //TODO: set camera/player Position;
            std::cout << "TODO: set player Position to"
                      << " x = " << pos.x()
                      << ", y = " << pos.y()
                      << ", z = " << pos.z() << std::endl;
        }
        else
        {
            std::cout << "WARNING: unknown scene property '"
                      << tag.toStdString() << "'"
                      << std::endl;
        }
    }

    cancelAllAnimations();
    // Apply transformations to objects
    m_rootGroup.updateWorld();
}

//------------------------------------------------------------------------------
void Scene::readObjectTreeFromDom(ObjectGroup *root, const QDomElement &domElement)
{
    for (auto child = domElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        // TODO have `Object` and `ObjectGroup` children
        QString tag = child.tagName();

        if (tag == "Object")
        {
            QString name = child.attribute("name", "unnamed object");
            QString modelName = child.attribute("model");
            QString event = child.attribute("interactionEvent");

            auto object = createObject(modelName.toStdString(), root);
            object->setName(name);
            object->setInteractionEvent(event);

            for (auto child2 = child.firstChildElement(); !child2.isNull(); child2 = child2.nextSiblingElement())
            {
                QString tag2 = child2.tagName();
                if (tag2 == "Position")
                {
                    auto position = readVectorFromDom(child2);
                    object->setPosition(position);
                }
                else if (tag2 == "Rotation")
                {
                    auto rotation = readVectorFromDom(child2);
                    object->setRotation(rotation);
                }
                else if (tag2 == "Scaling")
                {
                    auto scaling = readVectorFromDom(child2, QVector3D(1, 1, 1));
                    object->setScaling(scaling);
                }
                else if (tag2 == "SpecularColor")
                {
                    auto color = readVectorFromDom(child2, QVector3D(0.5, 0.5, 0.5));
                    object->setSpecularColor(color);
                }
                else if (tag2 == "DiffuseColor")
                {
                    auto color = readVectorFromDom(child2, QVector3D(0.5, 0.5, 0.5));
                    object->setDiffuseColor(color);
                }
                else if (tag2 == "AmbientColor")
                {
                    auto color = readVectorFromDom(child2, QVector3D(0.4, 0.4, 0.4));
                    object->setAmbientColor(color);
                }
            }
        }
        else if(tag == "ObjectGroup")
        {
            QString name = child.attribute("name", "unnamed group");

            auto objectGroup = createObjectGroup(name.toStdString(), root);

            for (auto child2 = child.firstChildElement(); !child2.isNull(); child2 = child2.nextSiblingElement())
            {
                QString tag2 = child2.tagName();
                if (tag2 == "Position")
                {
                    auto position = readVectorFromDom(child2);
                    objectGroup->setPosition(position);
                }
                else if (tag2 == "Rotation")
                {
                    auto rotation = readVectorFromDom(child2);
                    objectGroup->setRotation(rotation);
                }
                else if (tag2 == "Scaling")
                {
                    auto scaling = readVectorFromDom(child2, QVector3D(1, 1, 1));
                    objectGroup->setScaling(scaling);
                }
            }

            // Build object tree recursively
            readObjectTreeFromDom(objectGroup, child);
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
                      << tag.toStdString()
                      << "' in Object Tree"
                      << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
void Scene::readModelsFromDom(const QDomElement &domElement)
{
    for (auto child = domElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "Model")
        {
            QString name = child.attribute("name","");
            QString path = child.attribute("path", "");

            //test if object has valid parameters:
            if (path.length() == 0)
            {
                std::cout << "WARNING: no filepath for model"
                << "' " << name.toStdString() << "' given"
                << std::endl;
            }
            else
            {
                if (name.length() > 0)
                {
                    Model *model = new Model(path.toStdString(), name.toStdString());
                    addModel(std::unique_ptr<Model>(model));
                }
                else
                {
                    // If no name is given it will be automatically
                    // generated by the Model consructor
                    Model *model = new Model(path.toStdString());
                    addModel(std::unique_ptr<Model>(model));
                }
            }
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
            << child.tagName().toStdString()
            << "' in Model List"
            << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
void Scene::readEventsFromDom(const QDomElement &domElem)
{
    for (auto child = domElem.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "Event")
        {
            auto event = std::unique_ptr<Event>(new Event(m_globalState.get(), child));
            m_globalState->addEvent(std::move(event));
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
            << child.tagName().toStdString()
            << "' in Events List"
            << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
void Scene::readAttributesFromDom(const QDomElement &domElem)
{
    for (auto child = domElem.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "Attribute")
        {
            QString type = child.attribute("type");
            QString key = child.attribute("key");

            switch (QMetaType::type(type.toStdString().c_str()))
            {
                case QMetaType::Bool:
                {
                    QString valueStr = child.attribute("value");
                    m_globalState->setValue(key,
                                            valueStr == "true");
                    break;
                }
                case QMetaType::Int:
                {
                    QString valueStr = child.attribute("value");
                    m_globalState->setValue(key,
                                            valueStr.toInt());
                    break;
                }
                case QMetaType::Float:
                {
                    QString valueStr = child.attribute("value");
                    m_globalState->setValue(key,
                                            valueStr.toFloat());
                    break;
                }
                case QMetaType::QVector3D:
                {
                    // TODO
                }
                default:
                {
                    std::cout << "WARNING: Unknown attribute type '"
                    << type.toStdString()
                    << std::endl;
                }
            }
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
            << child.tagName().toStdString()
            << "' in Event List"
            << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
void Scene::readAnimatorsFromDom(const QDomElement &domElem)
{
    /*
    for (auto child = domElem.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "Animator")
        {
            QString type = child.attribute("type", "");
            QString objectName = child.attribute("object", "");

            float animationTime = child.attribute("time", "").toFloat();
            QString key = child.attribute("key", "");

            QString interpolation = child.attribute("interpolation", "");

            ObjectBase *object = findObjectByName(&m_rootGroup, objectName);
            if (!object)
            {
                std::cout << "Warning: object " << key.toStdString()
                          << "not found. Skipping it's " << type.toStdString()
                          << "-Animator"
                          << std::endl;
                continue;
            }

            switch(qStringToAnimation.at(type))
            {
            case AnimationType::Position:
            {
                addAnimator(std::unique_ptr<AnimatorBase>(new PositionAnimator(
                                                          object,
                                                          m_globalState.get(),
                                                          key,
                                                          qStringToInterpolation.at(interpolation),
                                                          animationTime)));
                break;
            }
            case AnimationType::Rotation:
            {
                addAnimator(std::unique_ptr<AnimatorBase>(new RotationAnimator(
                                                          object,
                                                          m_globalState.get(),
                                                          key,
                                                          qStringToInterpolation.at(interpolation),
                                                          animationTime)));
                break;
            }
            case AnimationType::Scaling:
            default:
            {
                std::cout << "Warning: unknown Animator type" << std::endl;
            }
            }
        }
        else
        {
            std::cout << "WARNING: Unexpected tag '"
            << child.tagName().toStdString()
            << "' in Animator List"
            << std::endl;
        }
    }
     */
}

//------------------------------------------------------------------------------
QVector3D Scene::readVectorFromDom(const QDomElement &domElement, const QVector3D &defValue)
{
    float x = domElement.hasAttribute("x") ? domElement.attribute("x").toFloat() : defValue.x();
    float y = domElement.hasAttribute("y") ? domElement.attribute("y").toFloat() : defValue.y();
    float z = domElement.hasAttribute("z") ? domElement.attribute("z").toFloat() : defValue.z();
    return QVector3D(x, y, z);
}

//------------------------------------------------------------------------------
void Scene::saveToFile(const QString &filename)
{
    // Example: https://gist.github.com/lamprosg/2133804

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        std::cerr << "Could not write to file '" << filename.toStdString() << "'"
                  << std::endl;
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    // Write header (= root element)
    xmlWriter.writeStartElement("Scene");
    xmlWriter.writeAttribute("name", m_sceneName);
    xmlWriter.writeAttribute("version", m_sceneVersion);
    xmlWriter.writeAttribute("author", m_sceneAuthor);

    // Write models:
    xmlWriter.writeStartElement("Models");
    for (auto &mapItem : getModels())
    {
        Model *model = mapItem.second.get();
        xmlWriter.writeStartElement("Model");
        xmlWriter.writeAttribute("name", QString::fromStdString(model->getName()));
        xmlWriter.writeAttribute("path", QString::fromStdString(model->getFilename()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    // Write object tree:
    xmlWriter.writeStartElement("Objects");
    writeObjectTree(&m_rootGroup, xmlWriter);
    xmlWriter.writeEndElement();

    // Write attributes:
    xmlWriter.writeStartElement("Attributes");
    for (auto &attribute : m_globalState->getAttributes())
    {
        writeAttribute(xmlWriter, attribute.first, attribute.second);
    }
    xmlWriter.writeEndElement();

    // Events:
    xmlWriter.writeStartElement("Events");
    for (auto &event : m_globalState->getEvents())
    {
        event->writeToXml(xmlWriter);
    }
    xmlWriter.writeEndElement();

    // Animators:
    xmlWriter.writeStartElement("Animators");

    for (const auto &anim : m_animations)
    {
        // TODO
    }

    xmlWriter.writeEndElement();

    //and the little other properties:

    //light:
    xmlWriter.writeStartElement("DirectionalLight");
    writeVectorToXml(getDirectionalLightDirection(), xmlWriter);
    QVector3D lightColor = getLightColor();
    xmlWriter.writeAttribute("r", QString::number(lightColor.x()));
    xmlWriter.writeAttribute("g", QString::number(lightColor.y()));
    xmlWriter.writeAttribute("b", QString::number(lightColor.z()));
    xmlWriter.writeEndElement();

    //player:
    xmlWriter.writeStartElement("Player");
    writeVectorToXml(QVector3D(0, 0, 0), xmlWriter); //TODO
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}

//------------------------------------------------------------------------------
void Scene::writeObjectTree(ObjectGroup *root, QXmlStreamWriter &writer)
{
    // Write child groups:
    for (ObjectGroup *group : root->getGroups())
    {
        writer.writeStartElement("ObjectGroup");

        writer.writeAttribute("name", group->getName());

        writer.writeStartElement("Position");
        writeVectorToXml(group->getPosition(), writer);
        writer.writeEndElement();

        writer.writeStartElement("Rotation");
        writeVectorToXml(group->getRotation(), writer);
        writer.writeEndElement();

        writer.writeStartElement("Scaling");
        writeVectorToXml(group->getScaling(), writer);
        writer.writeEndElement();

        // Write object tree recursively:
        writeObjectTree(group, writer);

        writer.writeEndElement();
    }

    // Write child objects:
    for (Object *object : root->getObjects())
    {
        QString modelName = QString::fromStdString(object->getModel()->getName());

        writer.writeStartElement("Object");

        writer.writeAttribute("name", object->getName());
        writer.writeAttribute("model", modelName);
        writer.writeAttribute("interactionEvent", object->getInteractionEvent());

        writer.writeStartElement("Position");
        writeVectorToXml(object->getPosition(), writer);
        writer.writeEndElement();

        writer.writeStartElement("Rotation");
        writeVectorToXml(object->getRotation(), writer);
        writer.writeEndElement();

        writer.writeStartElement("Scaling");
        writeVectorToXml(object->getScaling(), writer);
        writer.writeEndElement();

        writer.writeStartElement("SpecularColor");
        writeVectorToXml(object->getSpecularColor(), writer);
        writer.writeEndElement();

        writer.writeStartElement("DiffuseColor");
        writeVectorToXml(object->getDiffuseColor(), writer);
        writer.writeEndElement();

        writer.writeStartElement("AmbientColor");
        writeVectorToXml(object->getAmbientColor(), writer);
        writer.writeEndElement();

        writer.writeEndElement();
    }
}

//------------------------------------------------------------------------------
void Scene::writeAttribute(QXmlStreamWriter &writer, const QString &key, const QVariant &value)
{
    writer.writeStartElement("Attribute");

    writer.writeAttribute("key", key);
    writer.writeAttribute("type", value.typeName());

    switch((QMetaType::Type)value.type())
    {
        case QMetaType::Bool:
        {
            bool v = value.toBool();
            writer.writeAttribute("value", v ? "true" : "false");
            break;
        }
        case QMetaType::Int:
        {
            int v = value.toInt();
            writer.writeAttribute("value", QString::number(v));
            break;
        }
        case QMetaType::Float:
        {
            float v = value.toFloat();
            writer.writeAttribute("value", QString::number(v));
            break;
        }
        case QMetaType::QVector3D:
        {
            QVector3D v = value.value<QVector3D>();
            writer.writeAttribute("x", QString::number(v[0]));
            writer.writeAttribute("y", QString::number(v[1]));
            writer.writeAttribute("z", QString::number(v[2]));
            break;
        }
        default:
        {
            std::cout << "WARNING: Unknown attribute type '"
            << QMetaType::typeName(value.type())
            << "' in GlobalState"
            << std::endl;
        }
    }

    writer.writeEndElement();
}

//------------------------------------------------------------------------------
void Scene::writeVectorToXml(const QVector3D &pos, QXmlStreamWriter &writer)
{
    writer.writeAttribute("x", QString::number(pos.x()));
    writer.writeAttribute("y", QString::number(pos.y()));
    writer.writeAttribute("z", QString::number(pos.z()));
}

//------------------------------------------------------------------------------
void Scene::setCameraView(const QMatrix4x4 &camera)
{
    m_camera = camera;
}

//------------------------------------------------------------------------------
void Scene::setCameraProjection(const QMatrix4x4 &proj)
{
    m_proj = proj;
}

//------------------------------------------------------------------------------
QMatrix4x4 &Scene::getCameraView()
{
    return m_camera;
}

//------------------------------------------------------------------------------
QMatrix4x4 &Scene::getCameraProjection()
{
    return m_proj;
}

//------------------------------------------------------------------------------
void Scene::addModel(std::unique_ptr<Model> model)
{
    assert(m_models.find(model->getName()) == m_models.end());
    m_models[model->getName()].swap(model);
}

//------------------------------------------------------------------------------
void Scene::removeModel(const std::string &modelName)
{
    assert(m_models.find(modelName) != m_models.end());
    m_models.erase(modelName);
}

//------------------------------------------------------------------------------
void Scene::addAnimation(std::unique_ptr<AnimationBase> animation)
{
    m_animations.push_back(std::move(animation));
}

//------------------------------------------------------------------------------
void Scene::deleteAnimation(AnimationBase *animation)
{
    for (auto it = m_animations.begin(); it != m_animations.end(); it++)
    {
        if (it->get() == animation)
        {
            m_animations.erase(it);
            break;
        }
    }
}

//------------------------------------------------------------------------------
void Scene::performAnimations(IObjectBaseObserver *listener)
{
    // TODO
    for (const auto &animation : m_animations)
    {
        // TODO what happens if the animation deletes itself and invalidates the animators list?!
        animation->tick(0);
        if (listener)
        {
            listener->notify(animation->getObject());
        }
    }
}

//------------------------------------------------------------------------------
void Scene::cancelAllAnimations()
{
    for (auto const &animation : this->getAnimations())
    {
        // TODO what happens if the animation deletes itself and invalidates the animators list?!
        animation->cancelAnimation();
    }
}

//------------------------------------------------------------------------------
Object *Scene::createObject(const std::string &modelName, ObjectGroup *parent)
{
    if (!parent)
        parent = &m_rootGroup;

    assert(m_models.find(modelName) != m_models.end());
    Model *model = m_models[modelName].get();

    auto object = new Object(model);
    object->setName(QString::fromStdString(modelName));

    m_objects.push_back(object);

    parent->addObject(std::unique_ptr<Object>(object));
    return object;
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::createObjectGroup(const std::string &name, ObjectGroup *parent)
{
    if (!parent)
    {
        parent = &m_rootGroup;
    }

    auto newGroup = new ObjectGroup();
    newGroup->setName(QString::fromStdString(name));

    parent->addObjectGroup(std::unique_ptr<ObjectGroup>(newGroup));
    return newGroup;
}

//------------------------------------------------------------------------------
EditorObject *Scene::createEditorObject(const std::string &name, Model *model)
{
    auto object = new EditorObject(model);
    object->setName(QString::fromStdString(name));

    m_editorObjectRootGroup.addObject(std::unique_ptr<Object>(object));

    return object;
}

//------------------------------------------------------------------------------
range<Scene::ObjectIterator> Scene::getObjects()
{
    return range<ObjectIterator>(m_objects.cbegin(),m_objects.cend());
}

//------------------------------------------------------------------------------
range<Scene::AnimationIterator> Scene::getAnimations()
{
    return createRange(m_animations.cbegin(), m_animations.cend());
}

//------------------------------------------------------------------------------
range<ObjectGroup::object_iterator_type> Scene::getEditorObjects()
{
    return m_editorObjectRootGroup.getObjects();
}

//------------------------------------------------------------------------------
range<Scene::ModelIterator> Scene::getModels()
{
    return range<ModelIterator>(m_models.cbegin(), m_models.cend());
}

//------------------------------------------------------------------------------
QVector3D &Scene::getDirectionalLightDirection()
{
    return m_directionalLightDirection;
}

//------------------------------------------------------------------------------
QVector3D &Scene::getLightColor()
{
    return m_lightColor;
}

//------------------------------------------------------------------------------
void Scene::setLightColor(const QVector3D &color)
{
    this->m_lightColor = color;
}

//------------------------------------------------------------------------------
void Scene::setDirectionalLightDirection(const QVector3D &direction)
{
    this->m_directionalLightDirection = direction;
}

//------------------------------------------------------------------------------
ObjectGroup *Scene::getSceneRoot()
{
    return &m_rootGroup;
}

//------------------------------------------------------------------------------
void Scene::updateObjectList()
{
    m_objects.clear();
    ObjectGroup *root = &m_rootGroup;
    addToObjectList(root);
}

//------------------------------------------------------------------------------
void Scene::addToObjectList(ObjectGroup *root)
{
    for (Object *object : root->getObjects())
    {
        m_objects.push_back(object);
    }
    for (ObjectGroup *group : root->getGroups())
    {
        addToObjectList(group);
    }
}

//------------------------------------------------------------------------------
ObjectBase *Scene::findObjectByName(ObjectGroup *root, const QString &name)
{

    for (auto child : root->getObjects())
    {
        if (child->getName() == name)
        {
            return child;
        }
    }
    for (auto childGrp : root->getGroups())
    {
         ObjectBase *searchedObject = findObjectByName(childGrp, name);
         if (searchedObject)
         {
             return searchedObject;
         }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Model *Scene::getModel(const std::string &modelName)
{
    if (modelName.length() == 0)
    {
        return nullptr;
    }
    if(m_models.find(modelName) == m_models.end())
    {
        return nullptr;
    }
    return m_models[modelName].get();
}

//------------------------------------------------------------------------------
GlobalState *Scene::getGlobalState()
{
    return m_globalState.get();
}

//------------------------------------------------------------------------------
void Scene::setName(const QString &name)
{
    m_sceneName = name;
}

//------------------------------------------------------------------------------
void Scene::setVersion(const QString &version)
{
    m_sceneVersion = version;
}

//------------------------------------------------------------------------------
void Scene::setAuthor(const QString &author)
{
    m_sceneAuthor = author;
}

//------------------------------------------------------------------------------
const QString &Scene::getName() const
{
    return m_sceneName;
}

//------------------------------------------------------------------------------
const QString &Scene::getVersion() const
{
    return m_sceneVersion;
}

//------------------------------------------------------------------------------
const QString &Scene::getAuthor() const
{
    return m_sceneAuthor;
}
