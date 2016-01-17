#ifndef UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
#define UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H

#include <memory>

#include <QWidget>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

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
    void currentObjectChanged();

    void applyValues();

private:
    void generateWidgets();
    void connectStuff();

    void updateCurrentObject(Object *object);
    void updateCurrentObjectGroup(ObjectGroup *objectGroup);
    void updateCurrentObjectBase(ObjectBase *object);

    void applyCurrentObject(Object *object);
    void applyCurrentObjectGroup(ObjectGroup *objectGroup);
    void applyCurrentObjectBase(ObjectBase *object);

    void fillModelSelection();

    QLineEdit *m_name;

    QComboBox *m_modelSelection;

    QLineEdit *m_interactionEvent;

    QDoubleSpinBox *m_posX;
    QDoubleSpinBox *m_posY;
    QDoubleSpinBox *m_posZ;

    QDoubleSpinBox *m_rotYaw;
    QDoubleSpinBox *m_rotPitch;
    QDoubleSpinBox *m_rotRoll;

    QDoubleSpinBox *m_scaleX;
    QDoubleSpinBox *m_scaleY;
    QDoubleSpinBox *m_scaleZ;

    QDoubleSpinBox *m_ambientRed;
    QDoubleSpinBox *m_ambientGreen;
    QDoubleSpinBox *m_ambientBlue;

    QDoubleSpinBox *m_diffuseRed;
    QDoubleSpinBox *m_diffuseGreen;
    QDoubleSpinBox *m_diffuseBlue;

    QDoubleSpinBox *m_specularRed;
    QDoubleSpinBox *m_specularGreen;
    QDoubleSpinBox *m_specularBlue;

    QFormLayout *m_layout;

    QDoubleSpinBox *createNumericField(const QString &name);

    ObjectBase *m_currentObject;

    std::shared_ptr<SceneEditorGame> m_game;

    // necessary for avoiding a feedback loop
    // when property widgets are updated by the object
    bool m_objectPropertiesLocked;
};

#endif // UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
