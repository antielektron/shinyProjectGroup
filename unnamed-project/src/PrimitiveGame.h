#ifndef UNNAMED_PROJECT_PRIMITIVE_GAME_H
#define UNNAMED_PROJECT_PRIMITIVE_GAME_H

#include <QMatrix4x4>

#include "IGame.h"
#include "Scene/Scene.h"

class PrimitiveGame : public IGame
{
public:
    virtual void initialize() override;
    virtual void resize(int width, int height) override;
    virtual void tick() override;

    virtual Scene *getScene() override;

	virtual void onDoubleClick() override;

	virtual void onMouseButtonDown(int button) override;
	virtual void onMouseButtonUp(int button) override;
	virtual void onMouseMove(int x, int y) override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

private:
    std::unique_ptr<Scene> m_scene;

    Object *m_dummy;
    float r;
	float rotY = 0;
	float rotX = 0;
	float pos_height = 0;
	

	QVector3D m_position;
	QVector3D m_centre;
	QMatrix4x4 posMatrix;

	void updatePosMatrix(QVector3D deltaPos);
	

};

#endif // UNNAMED_PROJECT_PRIMITIVE_GAME_H
