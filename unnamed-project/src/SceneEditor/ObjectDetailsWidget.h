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
    void onModelsChanged();
    void onCurrentObjectChanged();

    void applyClicked();

private:
    void updateCurrentObject(Object *object);
    void updateCurrentObjectGroup(ObjectGroup *objectGroup);
    void updateCurrentObjectBase(ObjectBase *object);

    void applyCurrentObject(Object *object);
    void applyCurrentObjectGroup(ObjectGroup *objectGroup);
    void applyCurrentObjectBase(ObjectBase *object);

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

    Object *m_currentObject;
    ObjectGroup *m_currentObjectGroup;

    std::vector<Model *> m_models;

    std::shared_ptr<SceneEditorGame> m_game;
};

#endif // UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
