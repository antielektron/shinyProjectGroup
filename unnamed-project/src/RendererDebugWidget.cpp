#include "RendererDebugWidget.h"

#include "Renderer.h"

#include <QBoxLayout>
#include <QGroupBox>

RendererDebugWidget::RendererDebugWidget(Renderer *renderer, QWidget *parent) :
        QWidget(parent),
        m_renderer(renderer)
{
    generateWidgets();
    connectStuff();

    m_colorizeCascades->setChecked(m_renderer->getColorizeCascades());

    float min = m_cascadedShadowMapsLambda->minimum();
    float max = m_cascadedShadowMapsLambda->maximum();
    float value = m_renderer->getCascadedShadowMapsLambda();

    m_cascadedShadowMapsLambda->setValue(min + value * (max - min));

    switch (m_renderer->getCascadeStrategy())
    {
        case Renderer::CascadeStrategy::CascadedShadowMaps:
            m_cascadedShadowMaps->setChecked(true);
            break;
        case Renderer::CascadeStrategy::SampleDistributionShadowMaps:
            m_sampleDistributionShadowMaps->setChecked(true);
            break;
    }
}

RendererDebugWidget::~RendererDebugWidget()
{}

void RendererDebugWidget::generateWidgets()
{
    auto layout = new QVBoxLayout(this);
    this->setLayout(layout);

    auto sdsmOptions = generateSDSMOptions();
    layout->addWidget(sdsmOptions);

    // TODO create volumetric obscurance options
}

QWidget *RendererDebugWidget::generateSDSMOptions()
{
    auto sdsmOptions = new QGroupBox("SDSM Options", this);

    auto layout = new QVBoxLayout(sdsmOptions);
    sdsmOptions->setLayout(layout);

    m_colorizeCascades = new QCheckBox("Colorize Cascades", sdsmOptions);
    layout->addWidget(m_colorizeCascades);

    m_cascadedShadowMapsLambda = new QSlider(Qt::Horizontal, sdsmOptions);
    layout->addWidget(m_cascadedShadowMapsLambda);
    m_cascadedShadowMapsLambda->setRange(0, 100);
    m_cascadedShadowMapsLambda->setTickInterval(1);

    m_cascadedShadowMaps = new QRadioButton("CSM", sdsmOptions);
    layout->addWidget(m_cascadedShadowMaps);

    m_sampleDistributionShadowMaps = new QRadioButton("SDSM", sdsmOptions);
    layout->addWidget(m_sampleDistributionShadowMaps);

    return sdsmOptions;
}

void RendererDebugWidget::connectStuff()
{
    connect(m_colorizeCascades, SIGNAL(stateChanged(int)), this, SLOT(onColorCascadesChanged()));
    connect(m_cascadedShadowMapsLambda, SIGNAL(valueChanged(int)), this, SLOT(onCascadedShadowMapsLambdaChanged()));
    connect(m_cascadedShadowMaps, SIGNAL(clicked()), this, SLOT(onCascadesStrategyChanged()));
    connect(m_sampleDistributionShadowMaps, SIGNAL(clicked()), this, SLOT(onCascadesStrategyChanged()));
}

void RendererDebugWidget::onColorCascadesChanged()
{
    m_renderer->setColorizeCascades(m_colorizeCascades->isChecked());
}

void RendererDebugWidget::onCascadedShadowMapsLambdaChanged()
{
    float value = m_cascadedShadowMapsLambda->value();
    float min = m_cascadedShadowMapsLambda->minimum();
    float max = m_cascadedShadowMapsLambda->maximum();

    m_renderer->setCascadedShadowMapsLambda((value - min) / (max - min));
}

void RendererDebugWidget::onCascadesStrategyChanged()
{
    if (m_sampleDistributionShadowMaps->isChecked())
    {
        m_renderer->setCascadeStrategy(Renderer::CascadeStrategy::SampleDistributionShadowMaps);
    }
    else if (m_cascadedShadowMaps->isChecked())
    {
        m_renderer->setCascadeStrategy(Renderer::CascadeStrategy::CascadedShadowMaps);
    }
}
