#ifndef UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_TRIGGER_ACTION_WIDGET_H
#define UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_TRIGGER_ACTION_WIDGET_H

#include <memory>

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class SceneEditorGame;
class TriggerAction;

class TriggerActionWidget : public QWidget
{
    Q_OBJECT
public:
    TriggerActionWidget(std::shared_ptr<SceneEditorGame> game, TriggerAction *action, QWidget *parent = nullptr);
    virtual ~TriggerActionWidget();

protected slots:
    void onApplyClicked();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void generateWidgets();
    void connectStuff();

    std::shared_ptr<SceneEditorGame> m_game;
    TriggerAction *m_action;

    QFormLayout *m_layout;

    QLineEdit *m_name;
    QLineEdit *m_delay;

    QPushButton *m_apply;
};

#endif // UNNAMED_PROJECT_SCENE_EDITOR_GAME_LOGIC_TRIGGER_ACTION_WIDGET_H
