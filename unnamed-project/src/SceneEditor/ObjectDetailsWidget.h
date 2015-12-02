#ifndef UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
#define UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H

#include <memory>

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

#include <QFormLayout>

class SceneEditorGame;

class ObjectDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    ObjectDetailsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = 0);
    virtual ~ObjectDetailsWidget() {}

private:
    QComboBox *m_modelSelection;

    QLineEdit *m_posX;
    QLineEdit *m_posY;
    QLineEdit *m_posZ;

    QLineEdit *m_rotYaw;
    QLineEdit *m_rotPitch;
    QLineEdit *m_rotRoll;

    QFormLayout *m_layout;

    QLineEdit *createNumericField(const QString &name);

    std::shared_ptr<SceneEditorGame> m_game;
};

#endif // UNNAMED_PROJECT_OBJECT_DETAILS_WIDGET_H
