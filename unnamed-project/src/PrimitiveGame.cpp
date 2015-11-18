#include "PrimitiveGame.h"

#include <iostream>

void PrimitiveGame::initialize()
{
    m_scene = std::unique_ptr<Scene>(new Scene());

    // static camera
    m_position = QVector3D(0, 0, 5);
	updatePosMatrix(QVector3D(0,0,0));



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
	QVector3D deltaPos(0, 0, 0);

	if (key == Qt::Key_Right) 
	{
        rotY -= 1.0;
		 /*QMatrix4x4 matrix44x;
		matrix44x.setToIdentity();
		matrix44x.rotate(rotY, 0, 1, 0);
		QVector3D alpha = matrix44x*QVector3D(0, 0, 1);
		m_centre = m_position + alpha;
		m_scene->getCamera().setToIdentity();
		m_scene->getCamera().lookAt(m_position, m_centre, QVector3D(0, 1, 0));
		std::cout << rotY << " alpha-Wert "; */
		
	}
	else if (key == Qt::Key_Left)
	{
        rotY += 1.0;
	}
	else if (key == Qt::Key_Up)
	{
        rotX += 1.0;
	}
	else if (key == Qt::Key_Down)
	{
        rotX -= 1.0;
	}
    else if (key == Qt::Key_S)
	{
		deltaPos += QVector3D(0, 0, 0.5);
	}
    else if (key == Qt::Key_W)
	{
		deltaPos += QVector3D(0, 0, -0.5);
	}
    else if (key == Qt::Key_D)
	{
		deltaPos += QVector3D(0.5, 0, 0);
	}
    else if (key == Qt::Key_A)
	{
		deltaPos += QVector3D(-0.5, 0, 0);
	}
	else if (key == Qt::Key_Space)
	{
		m_position = QVector3D(0, 0, 0);
		rotX = 0;
		rotY = 0;
	}
	updatePosMatrix(deltaPos);
}

void PrimitiveGame::updatePosMatrix(QVector3D deltaPos) 
{
	QMatrix4x4 &camera = m_scene->getCamera();
	QMatrix4x4 translate;
	QMatrix4x4 xRotation;
	QMatrix4x4 yRotation;

    xRotation.setToIdentity();
    yRotation.setToIdentity();
    translate.setToIdentity();

	camera.setToIdentity();
	xRotation.rotate(rotX, 1, 0, 0);
	yRotation.rotate(rotY, 0, 1, 0);
	translate.translate(m_position);

	m_position += (xRotation * yRotation) * deltaPos;

    xRotation.setToIdentity();
    yRotation.setToIdentity();
    translate.setToIdentity();

    xRotation.rotate(-rotX, 1, 0, 0);
    yRotation.rotate(-rotY, 0, 1, 0);
    translate.translate(-1 * m_position);

	camera = ((xRotation * yRotation *translate));
	
}
