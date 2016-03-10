#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"
#include "GameLogic/Event.h"
#include <cmath>
#include <iostream>

SceneEditorGame::SceneEditorGame() : 
		QObject(nullptr), 
		m_currentObject(nullptr),
        m_scenefile("level/test.xml")
{
    m_logicRunning = false;
    m_logicPaused = false;
}

SceneEditorGame::SceneEditorGame(const QString &scenefile) :
        QObject(nullptr),
        m_currentObject(nullptr),
        m_scenefile(scenefile)
{
    m_logicRunning = false;
    m_logicPaused = false;
}

void SceneEditorGame::initialize()
{
	// call parent
	IGame::initialize();

    // Load a test level by default or empty scene
    reset(std::unique_ptr<Scene>(new Scene(m_scenefile)));
}

void SceneEditorGame::reset(std::unique_ptr<Scene> scene)
{
    m_scene = std::move(scene);

    m_currentObject = nullptr;

    m_time = 0;

	m_scene->getCameraView().translate(0., 0., -10);

    m_scene->getSceneRoot()->updateWorld();

    createIndicatorObject();
    m_indicatorObject->makeInvisible();

    m_logicRunning = false;
    m_logicPaused = false;

    emit currentObjectChanged();
    emit modelsChanged();
    emit objectsChanged();
	emit sceneChanged();
}

void SceneEditorGame::resize(int width, int height)
{
    auto &proj = m_scene->getCameraProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.2f, 500.0f);
}

void SceneEditorGame::tick(float dt)
{
    m_deltaTime = dt;
    m_time += m_deltaTime;

    // TODO provide player position + time in "external fields"

    // run animataions and run handle game logic events
    if (m_logicRunning && !m_logicPaused)
    {
        // only trigger the tick event that is triggered every tick ;)
        m_scene->getGlobalState()->triggerEvent("tick");
        // m_scene->performAnimations();
        // TODO have to notify ui about changes in attributes!
    }

    // update indicator stuff
    m_extraIndicatorH = (std::sin(3.0f * m_time) + 1.0f) * 0.2f * m_indicatorScale;
    m_indicatorRotation += 90.f * m_deltaTime;
    if (m_indicatorRotation > 360.0f)
    {
        m_indicatorRotation -= 360.0f;
    }

    // TODO call the following only when needed!
    if (m_currentObject)
    {
        m_indicatorH = m_currentObject->getPosition()[1];
        if (m_currentObject->getObjectType() != ObjectType::ObjectGroup)
        {
            m_indicatorH += static_cast<Object *>(m_currentObject)->getModel()->getRadius();
        }
    }
    m_indicatorObject->getPosition()[1] = m_extraIndicatorH + m_indicatorH;
    m_indicatorObject->getRotation()[1] = m_indicatorRotation;
    m_indicatorObject->updateWorld();

	QVector3D deltaPos(0, 0, 0);

	if (!m_keyManager)
		return;

	m_keyManager->tick();

    float speed = 10.0f * m_deltaTime;

    if (m_keyManager->isKeyDown(Qt::Key_Control))
        speed *= 5;

	if (m_keyManager->shouldCatchMouse())
	{
		m_rotX += m_keyManager->getRelativeY() * .1;
		m_rotY += m_keyManager->getRelativeX() * .1;
	}

	if (m_keyManager->isKeyDown(Qt::Key_Right))
	{
		m_rotY += 0.5;
	}
	if (m_keyManager->isKeyDown(Qt::Key_Left))
	{
		m_rotY -= 0.5;
	}
	if (m_keyManager->isKeyDown(Qt::Key_Up))
	{
		m_rotX += 0.5;
	}
	if (m_keyManager->isKeyDown(Qt::Key_Down))
	{
		m_rotX -= 0.5;
	}
	if (m_keyManager->isKeyDown(Qt::Key_S))
	{
		deltaPos += QVector3D(0, 0, speed);
	}
	if (m_keyManager->isKeyDown(Qt::Key_W))
	{
		deltaPos += QVector3D(0, 0, -speed);
	}
	if (m_keyManager->isKeyDown(Qt::Key_D))
	{
		deltaPos += QVector3D(speed, 0, 0);
	}
	if (m_keyManager->isKeyDown(Qt::Key_A))
	{
		deltaPos += QVector3D(-speed, 0, 0);
	}
	if (m_keyManager->isKeyDown(Qt::Key_Space) || m_keyManager->isKeyDown(Qt::Key_Q))
	{
		deltaPos += QVector3D(0, speed, 0);
	}
	if (m_keyManager->isKeyDown(Qt::Key_Shift) || m_keyManager->isKeyDown(Qt::Key_Z) ||
		m_keyManager->isKeyDown(Qt::Key_Y))
	{
		deltaPos += QVector3D(0, -speed, 0);
	}

	if (m_keyManager->isKeyDown(Qt::Key_T))
	{
		if (m_currentObject)
		{
			m_currentObject->getPosition() += QVector3D(0.0,0.1,0.0);
		}
	}

	// Reset camera
	if (m_keyManager->isKeyDown(Qt::Key_R))
	{
		m_position = QVector3D(0, 0, 0);
		m_rotX = 0;
		m_rotY = 0;
	}

	if (m_rotX > 90.f)
		m_rotX = 90.f;
	if (m_rotX < -90.f)
		m_rotX = -90.f;

	updatePosMatrix(deltaPos);

	// Start/Stop catching mouse
	if (m_keyManager->isKeyPressed(Qt::Key_Escape))
	{
		m_keyManager->setCatchMouse(!m_keyManager->shouldCatchMouse());
	}
}

Scene *SceneEditorGame::getScene()
{
    return m_scene.get();
}

GlobalState *SceneEditorGame::getGlobalState()
{
    if (m_scene)
        return m_scene->getGlobalState();
    else
        return nullptr;
}

ObjectGroup *SceneEditorGame::getRootObject()
{
    return m_scene->getSceneRoot();
}

void SceneEditorGame::updatePosMatrix(QVector3D deltaPos)
{
	QMatrix4x4 &camera = m_scene->getCameraView();
	QMatrix4x4 translation;
	QMatrix4x4 xRotation;
	QMatrix4x4 yRotation;

	xRotation.rotate(m_rotX, 1, 0, 0);
	yRotation.rotate(m_rotY, 0, 1, 0);
	translation.translate(-1 * m_position);

	QMatrix4x4 rotation = xRotation * yRotation;

	m_position += rotation.transposed() * deltaPos;

	camera = rotation * translation;
}

void SceneEditorGame::notifyCurrentObjectChanged(ObjectBase *object)
{
	// Called when a new object was selected or the current object has changed.
    m_currentObject = object;

    if (object && object->getObjectType() != ObjectType::ObjectGroup)
    {
        m_indicatorObject->makeVisible();
        QVector3D indicatorPosition = object->getAbsolutePosition();
        Model *objModel = static_cast<Object *>(object)->getModel();
        m_indicatorH = objModel->getCenter().y() + objModel->getRadius();
        indicatorPosition += QVector3D(0,m_indicatorH,0);
        m_indicatorObject->setPosition(indicatorPosition);
        m_indicatorScale = objModel->getRadius() * 0.5f;
        m_indicatorObject->setScaling(
                    QVector3D(m_indicatorScale, m_indicatorScale, m_indicatorScale));
        m_indicatorObject->updateWorld();
        m_indicatorScale = objModel->getRadius();
    }
    else
    {
        m_indicatorObject->makeInvisible();
    }

    emit currentObjectChanged();
}

void SceneEditorGame::removeCurrentObject()
{
	if (m_currentObject)
	{
		m_currentObject->destroy();
        m_scene->updateObjectList();
        m_currentObject = nullptr;
        emit objectsChanged();
	}
}

ObjectBase *SceneEditorGame::getCurrentObject()
{
	return m_currentObject;
}

Model *SceneEditorGame::getModelByName(const std::string &modelName)
{
    return m_scene->getModel(modelName);
}

void SceneEditorGame::addModel(std::unique_ptr<Model> model)
{
    m_scene->addModel(std::move(model));
    emit modelsChanged();
}


const std::string &SceneEditorGame::getCurrentModel()
{
    return m_currentModelName;
}

void SceneEditorGame::setCurrentModel(const std::string &modelName)
{
    m_currentModelName = modelName;
    emit currentModelChanged();
}

void SceneEditorGame::removeCurrentModel()
{
    if (m_currentModelName.empty())
        return;

    m_scene->removeModel(m_currentModelName);
    m_currentModelName = "";
    emit modelsChanged();
    // a change in models implies a possible change in current model
    // emit currentModelChanged();
}

Object *SceneEditorGame::createObject(const std::string &modelName, ObjectGroup *parent)
{
	Object *obj = m_scene->createObject(modelName, parent);
    emit objectsChanged();
    return obj;
}

ObjectGroup *SceneEditorGame::createObjectGroup(const std::string &name, ObjectGroup *parent)
{
    ObjectGroup *objGrp = m_scene->createObjectGroup(name, parent);
    emit objectsChanged();
    return objGrp;
}

//------------------------------------------------------------------------------
void SceneEditorGame::runLogic()
{
    if (!m_logicRunning)
    {
        m_scene->cancelAllAnimations();
        m_scene->getGlobalState()->stash();
        m_logicRunning = true;
        m_logicPaused = false;
    }
}

//------------------------------------------------------------------------------
void SceneEditorGame::stopLogic()
{
    if (m_logicRunning)
    {
        m_scene->getGlobalState()->applyStash();
        m_logicRunning = false;
        m_logicPaused = false;
    }
    emit attributesChanged(); // TODO hm
}

//------------------------------------------------------------------------------
void SceneEditorGame::togglePauseLogic()
{
    if (m_logicRunning)
    {
        m_logicPaused = !m_logicPaused;
    }
}

//------------------------------------------------------------------------------
void SceneEditorGame::addAttribute(const QString &key, const QVariant &value)
{
    m_scene->getGlobalState()->setValue(key, value);
    emit attributesChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::deleteAttribute(const QString &key)
{
    m_scene->getGlobalState()->removeValue(key);
    emit attributesChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::notifyAttributeChanged()
{
    emit attributesChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::addEvent(std::unique_ptr<Event> event)
{
    m_scene->getGlobalState()->addEvent(std::move(event));
    emit eventsChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::deleteEvent(GlobalState::EventIterator iterator)
{
    m_scene->getGlobalState()->removeEvent(iterator);
    emit eventsInvalidated();
}

//------------------------------------------------------------------------------
void SceneEditorGame::notifyEventChanged()
{
    emit eventsChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::addAnimation(std::unique_ptr<AnimationBase> animation)
{
    m_scene->addAnimation(std::move(animation));
    emit animatorsChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::deleteAnimation(AnimationBase *animation)
{
    m_scene->deleteAnimation(animation);
    emit animatorsChanged();
}

//------------------------------------------------------------------------------
void SceneEditorGame::createIndicatorObject()
{
	// NOTE: this model should NOT be part of the scene!!!
    Model *model = new Model("models/editorIndicator.obj");
    m_indicatorObject = m_scene->createEditorObject("editor indicator", model);
}
