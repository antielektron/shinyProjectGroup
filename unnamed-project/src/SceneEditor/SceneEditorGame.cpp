#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"

SceneEditorGame::SceneEditorGame() : 
		QObject(nullptr), 
		m_currentObject(nullptr),
        m_keyManager(nullptr),
        m_initialized(false)
{
}

void SceneEditorGame::initialize()
{
    // Load a test level by default or empty scene
    reset(std::unique_ptr<Scene>(new Scene("level/test.xml")));
}

void SceneEditorGame::reset(std::unique_ptr<Scene> scene)
{
    m_scene = std::move(scene);

    m_initialized = true;

    m_currentObject = nullptr;

    m_scene->getCamera().translate(0., 0., -10);

    m_scene->getSceneRoot()->updateWorld();

    emit currentObjectChanged();
    emit modelsChanged();
    emit objectsChanged();
	emit sceneChanged();
    emit sceneReloaded();
}

void SceneEditorGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void SceneEditorGame::tick()
{
	QVector3D deltaPos(0, 0, 0);

	if (!m_keyManager)
		return;

	m_keyManager->tick();

	float speed = 10. / 60;

	if (m_keyManager->shouldCatchMouse())
	{
		rotX += m_keyManager->getRelativeY() * .1;
		rotY += m_keyManager->getRelativeX() * .1;
	}

	if (m_keyManager->isKeyPressed(Qt::Key_Right))
	{
		rotY += 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Left))
	{
		rotY -= 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Up))
	{
		rotX += 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Down))
	{
		rotX -= 0.5;
	}
	if (m_keyManager->isKeyPressed(Qt::Key_S))
	{
		deltaPos += QVector3D(0, 0, speed);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_W))
	{
		deltaPos += QVector3D(0, 0, -speed);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_D))
	{
		deltaPos += QVector3D(speed, 0, 0);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_A))
	{
		deltaPos += QVector3D(-speed, 0, 0);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_Space) || m_keyManager->isKeyPressed(Qt::Key_Q))
	{
		deltaPos += QVector3D(0, speed, 0);
	}
	
	if (m_keyManager->isKeyPressed(Qt::Key_Shift) || m_keyManager->isKeyPressed(Qt::Key_Z) || m_keyManager->isKeyPressed(Qt::Key_Y))
	{
		deltaPos += QVector3D(0, -speed, 0);
	}
	if (m_keyManager->isKeyPressed(Qt::Key_T))
	{
		if (m_currentObject)
		{
			m_currentObject->getPosition() += QVector3D(0.0,0.1,0.0);
		}
	}

	// Reset camera
	if (m_keyManager->isKeyPressed(Qt::Key_R))
	{
		m_position = QVector3D(0, 0, 0);
		rotX = 0;
		rotY = 0;
	}

	if (rotX > 90.f)
		rotX = 90.f;
	if (rotX < -90.f)
		rotX = -90.f;

	updatePosMatrix(deltaPos);

	// Start/Stop catching mouse
	if (m_keyManager->isKeyPressed(Qt::Key_Escape) && !m_wasEscDown)
	{
		m_keyManager->setCatchMouse(!m_keyManager->shouldCatchMouse());
	}
	m_wasEscDown = m_keyManager->isKeyPressed(Qt::Key_Escape);
}

Scene *SceneEditorGame::getScene()
{
    return m_scene.get();
}

void SceneEditorGame::setKeyManager(KeyManager *keyManager)
{
	this->m_keyManager = keyManager;
}

bool SceneEditorGame::isInitialized()
{
    return m_initialized;
}

ObjectGroup *SceneEditorGame::getRootObject()
{
    return m_scene->getSceneRoot();
}

void SceneEditorGame::updatePosMatrix(QVector3D deltaPos)
{
	QMatrix4x4 &camera = m_scene->getCamera();
	QMatrix4x4 translation;
	QMatrix4x4 xRotation;
	QMatrix4x4 yRotation;

	xRotation.rotate(rotX, 1, 0, 0);
	yRotation.rotate(rotY, 0, 1, 0);
	translation.translate(-1 * m_position);

	QMatrix4x4 rotation = xRotation * yRotation;

	m_position += rotation.transposed() * deltaPos;

	camera = rotation * translation;
}

void SceneEditorGame::currentObjectModified(ObjectBase* object)
{
	// Called when a new object was selected or the current object has changed.
    m_currentObject = object;
    emit currentObjectChanged();
}

void SceneEditorGame::getModels(std::vector<Model *> &models)
{
    for (auto &mapItem : m_scene->getModels())
    {
        models.push_back(mapItem.second.get());
    }
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

void SceneEditorGame::removeModel(const std::string &modelName)
{
    m_scene->removeModel(modelName);
    emit modelsChanged();
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
void SceneEditorGame::createIndicatorObject()
{
    m_indicatorModel.reset(new Model("models/editorIndicator"));
    m_indicatorObject.reset(new Object(m_indicatorModel.get()));
}

void SceneEditorGame::onCurrentObjectChanged(ObjectBase * object)
{
	m_currentObject = object;
}
