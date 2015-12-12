#ifndef UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
#define UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H

#include <memory>

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include <QFormLayout>

class SceneEditorGame;

class ObjectBase;
class Object;
class ObjectGroup;
class Model;

class ObjectDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    ObjectDetailsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = 0);
    virtual ~ObjectDetailsWidget() {}

public slots:
    void modelsChanged();
    void currentObjectChanged(ObjectBase *object);
    void applyClicked();

private:
    void updateCurrentObject(Object *object);
    void updateCurrentObjectGroup(ObjectGroup *objectGroup);
    void updateCurrentObjectBase(ObjectBase *object);

    void applyCurrentObject(Object *object);
    void applyCurrentObjectGroup(ObjectGroup *objectGroup);
    void applyCurrentObjectBase(ObjectBase *object);

    void fillModelSelection();

    QComboBox *m_modelSelection;

    QLineEdit *m_posX;
    QLineEdit *m_posY;
    QLineEdit *m_posZ;

    QLineEdit *m_rotYaw;
    QLineEdit *m_rotPitch;
    QLineEdit *m_rotRoll;

    QPushButton *m_applyButton;

    QFormLayout *m_layout;

    QLineEdit *createNumericField(const QString &name);

    ObjectBase *m_currentObject;

    std::vector<Model *> m_models;

    std::shared_ptr<SceneEditorGame> m_game;
};

#endif // UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
