#include "SceneEditor/GameLogic/ObjectAnimationActionBaseWidget.h"
#include "SceneEditor/SceneEditorGame.h"
#include "GameLogic/Factories/InterpolationCreatorFactory.h"

ObjectAnimationActionBaseWidget::ObjectAnimationActionBaseWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
        QWidget(parent),
        m_game(game)
{
    generateWidgets();
    connectStuff();
}

ObjectAnimationActionBaseWidget::~ObjectAnimationActionBaseWidget()
{}

void ObjectAnimationActionBaseWidget::generateWidgets()
{
    m_layout = new QFormLayout(this);
    this->setLayout(m_layout);

    m_type = new QLabel(this);
    m_layout->addRow("Type", m_type);

    m_name = new QLineEdit(this);
    m_layout->addRow("Object Name", m_name);

    m_exprX = new QLineEdit(this);
    m_layout->addRow("Expression X", m_exprX);

    m_exprY = new QLineEdit(this);
    m_layout->addRow("Expression Y", m_exprY);

    m_exprZ = new QLineEdit(this);
    m_layout->addRow("Expression Z", m_exprZ);

    m_interpolationType = new QComboBox(this);
    m_layout->addRow("Interpolation Type", m_interpolationType);

    // Fill list here, so that no on change events get triggered!
    std::vector<QString> interpolationTypes = Factory::getKnownInterpolationCreatorTypes();

    m_interpolationType->clear();
    for (auto &type : interpolationTypes)
    {
        m_interpolationType->addItem(type);
    }

    m_editInterpolation = new QPushButton("Edit Interpolation", this);
    m_layout->addRow(m_editInterpolation);

    m_apply = new QPushButton("Apply", this);
    m_layout->addRow(m_apply);
}

void ObjectAnimationActionBaseWidget::connectStuff()
{
    connect(m_apply, SIGNAL(clicked()), this, SLOT(onApplyClicked()));
    connect(m_editInterpolation, SIGNAL(clicked()), this, SLOT(onEditInterpolationClicked()));
    connect(m_interpolationType, SIGNAL(currentTextChanged(QString)), this, SLOT(onInterpolationTypeChanged()));

    // connect to game (close on any change)
    connect(m_game.get(), SIGNAL(eventsChanged()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(eventsInvalidated()), this, SLOT(close()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(close()));
}

void ObjectAnimationActionBaseWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    QWidget::closeEvent(event);
}
