#include "PrimitiveGame.h"

#include <iostream>

void PrimitiveGame::initialize()
{
    m_scene = std::unique_ptr<Scene>(new Scene());

    // static camera
    auto &camera = m_scene->getCamera();
	m_position = QVector3D(0, 2, -10);
	m_centre = QVector3D(0, 0, 0);
    camera.setToIdentity();
   // camera.translate(0, 0, -10);
    camera.lookAt(m_position, m_centre, QVector3D(0, 1, 0));

    //m_scene->addModel("test", std::unique_ptr<Model>(new Model("models/test.obj")));
    //m_scene->addModel("audi", std::unique_ptr<Model>(new Model("models/Audi_R8.obj")));
	m_scene->addModel("octo", std::unique_ptr<Model>(new Model("models/octonorm.obj")));
    m_dummy = m_scene->createObject("octo");

    m_dummy->getWorld().setToIdentity();
    r = 0.;

    //set directional light:
    m_scene->setDirectionalLightDirection(QVector3D(0.0,0.0,-1.0));
    m_scene->setLightColor(QVector3D(1.0,1.0,1.0));
}

void PrimitiveGame::resize(int width, int height)
{
    auto &proj = m_scene->getProjection();
    proj.setToIdentity();
    proj.perspective(45.0f, (float)width / height, 0.01f, 100.0f);
}

void PrimitiveGame::tick()
{
    auto &world = m_dummy->getWorld();
    world.setToIdentity();

    r+= 1.f;
    world.rotate(r, 0, 1, 0);
}

Scene *PrimitiveGame::getScene()
{
    return m_scene.get();
}

void PrimitiveGame::onDoubleClick()
{
	//m_scene->getCamera().translate(0, 0, -5);
	
}

void PrimitiveGame::onKeyEvent(int key)
{
	
	if (key == Qt::Key_Right) 
	{
		//m_position += QVector3D(0.0, 0.0, 0.0);
		rotY += 1.0;
		QMatrix4x4 matrix44x;
		matrix44x.setToIdentity();
		matrix44x.rotate(rotY, 0, 1, 0);
		QVector3D alpha = matrix44x*QVector3D(0, 0, 1);
		m_centre = m_centre + alpha;
		m_scene->getCamera().setToIdentity();
		m_scene->getCamera().lookAt(m_position, m_centre, QVector3D(0, 1, 0));
		
	}
	else if (key == Qt::Key_Left)
	{
		
		rotY += 1.0;
		QMatrix4x4 matrix44x;
		matrix44x.setToIdentity();
		matrix44x.rotate(rotY, 0, -1, 0);
		QVector3D alpha = matrix44x*QVector3D(0, 0, 1);
		m_centre = m_centre + alpha;
		m_scene->getCamera().setToIdentity();
		m_scene->getCamera().lookAt(m_position, m_centre, QVector3D(0, 1, 0));
	}
	else if (key == Qt::Key_Up)
	{
		m_position += QVector3D(0, 0, 0.5);
		m_centre += QVector3D(0, 0, 0);
		m_scene->getCamera().setToIdentity();
		m_scene->getCamera().lookAt(m_position, m_centre, QVector3D(0, 1, 0));
	}
	else if (key == Qt::Key_Down)
	{
		m_position -= QVector3D(0, 0, 0.5);
		m_centre -= QVector3D(0, 0, 0);
		m_scene->getCamera().setToIdentity();
		m_scene->getCamera().lookAt(m_position, m_centre, QVector3D(0, 1, 0));
	}
}
