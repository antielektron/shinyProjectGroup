#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GLOBAL_DETAILS_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GLOBAL_DETAILS_WIDGET_H

#include <memory>

#include <QWidget>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <QFormLayout>
#include <QLabel>

class SceneEditorGame;

class GlobalDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    GlobalDetailsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = 0);
    virtual ~GlobalDetailsWidget() {}

public slots:
    void sceneChanged();
    void valuesChanged();

    void onBrowseTriggered();

signals:
    void shaderConfigurationChanged(const QString &filename);

private:
    void updateUI();
    void applyScene();

    QDoubleSpinBox *m_lightPosX;
    QDoubleSpinBox *m_lightPosY;
    QDoubleSpinBox *m_lightPosZ;

    QDoubleSpinBox *m_lightColorR;
    QDoubleSpinBox *m_lightColorG;
    QDoubleSpinBox *m_lightColorB;

    QFormLayout *m_layout;

    // for file selection:
    QPushButton *m_browseButton;
    QLabel   *m_fileSelectionLabel;

    QDoubleSpinBox *createNumericField(const QString &name);

    std::shared_ptr<SceneEditorGame> m_game;

    // necessary for avoiding a feedback loop
    // when property widgets are updated by the object
    bool m_propertiesLocked;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GLOBAL_DETAILS_WIDGET_H
