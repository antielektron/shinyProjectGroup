#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INTERPOLATION_CREATOR_DURATION_BASE_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INTERPOLATION_CREATOR_DURATION_BASE_WIDGET_H

#include <memory>

#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class SceneEditorGame;

class InterpolationCreatorDurationBaseWidget : public QWidget
{
    Q_OBJECT
public:
    InterpolationCreatorDurationBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent = nullptr);
    virtual ~InterpolationCreatorDurationBaseWidget() {}

protected slots:
    virtual void onApplyClicked() = 0;

protected:
    virtual void closeEvent(QCloseEvent *event) override;

    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;

    QFormLayout *m_layout;

    QLabel *m_type;

    QLineEdit *m_duration;

    QPushButton *m_apply;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_INTERPOLATION_CREATOR_DURATION_BASE_WIDGET_H
