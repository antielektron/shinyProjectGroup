#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_SCENE_ANIMATION_ACTION_BASE_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_SCENE_ANIMATION_ACTION_BASE_WIDGET_H

#include <memory>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QComboBox>

class SceneEditorGame;

class SceneAnimationActionBaseWidget : public QWidget
{
    Q_OBJECT
public:
    SceneAnimationActionBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~SceneAnimationActionBaseWidget();

protected slots:
    virtual void onApplyClicked() = 0;
    virtual void onInterpolationTypeChanged() = 0;
    virtual void onEditInterpolationClicked() = 0;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;

    QFormLayout *m_layout;

    QLabel *m_type;

    // TODO retrieve labels in template class via traits!
    QLineEdit *m_exprX;
    QLineEdit *m_exprY;
    QLineEdit *m_exprZ;

    QComboBox *m_interpolationType;

    QPushButton *m_editInterpolation;

    QPushButton *m_apply;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_OBJECT_ANIMATION_ACTION_BASE_WIDGET_H
