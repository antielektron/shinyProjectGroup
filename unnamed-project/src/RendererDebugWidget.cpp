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

    m_filterShadowMap->setChecked(m_renderer->getFilterShadowMap());
    for (int i = 0; i < 5; i++)
    {
        if (m_renderer->getShadowMapSampleCount() == 1<<i)
        {
            m_shadowMapMsaa[i]->setChecked(true);
        }
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

    auto msmOptions = generateMSMOptions();
    layout->addWidget(msmOptions);

    auto voOptions = generateVOOptions();
    layout->addWidget(voOptions);
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

    m_lightView = new QCheckBox("Light View");
    layout->addWidget(m_lightView);

    m_awesomeCapture = new QCheckBox("Awesome Capture");
    layout->addWidget(m_awesomeCapture);


    return sdsmOptions;
}

QWidget *RendererDebugWidget::generateVOOptions()
{
    auto voOptions = new QGroupBox("VO variants", this);
    auto layout = new QVBoxLayout(voOptions);
    voOptions->setLayout(layout);

    m_lineVO = new QRadioButton("line sampling", voOptions);
    layout->addWidget(m_lineVO);

    m_varianceVO = new QRadioButton("variance area sampling", voOptions);
    layout->addWidget(m_varianceVO);

    m_momentVO = new QRadioButton("moment area sampling", voOptions);
    layout->addWidget(m_momentVO);

    return voOptions;
}

QWidget *RendererDebugWidget::generateMSMOptions()
{
    auto msmOptions = new QGroupBox("MSM Options", this);

    auto layout = new QVBoxLayout(msmOptions);
    msmOptions->setLayout(layout);

    m_filterShadowMap = new QCheckBox("Filter Shadowmap");
    layout->addWidget(m_filterShadowMap);


    for (int i = 0; i < 5; i++)
    {
        m_shadowMapMsaa[i] = new QRadioButton(QString::number(1<<i) + "x MSAA", msmOptions);
        layout->addWidget(m_shadowMapMsaa[i]);
    }

    return msmOptions;
}

void RendererDebugWidget::connectStuff()
{
    connect(m_colorizeCascades, SIGNAL(stateChanged(int)), this, SLOT(onColorCascadesChanged()));
    connect(m_cascadedShadowMapsLambda, SIGNAL(valueChanged(int)), this, SLOT(onCascadedShadowMapsLambdaChanged()));
    connect(m_cascadedShadowMaps, SIGNAL(clicked()), this, SLOT(onCascadesStrategyChanged()));
    connect(m_sampleDistributionShadowMaps, SIGNAL(clicked()), this, SLOT(onCascadesStrategyChanged()));
    connect(m_awesomeCapture, SIGNAL(stateChanged(int)), this, SLOT(onAwesomeCaptureChanged()));
    connect(m_lightView, SIGNAL(stateChanged(int)), this, SLOT(onLightViewChanged()));

    connect(m_lineVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));
    connect(m_varianceVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));
    connect(m_momentVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));

    connect(m_filterShadowMap, SIGNAL(stateChanged(int)), this, SLOT(onFilterShadowMapChanged()));
    for (int i = 0; i < 5; i++)
    {
        connect(m_shadowMapMsaa[i], SIGNAL(clicked()), this, SLOT(onShadowMapMsaaChanged()));
    }
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

void RendererDebugWidget::onAwesomeCaptureChanged()
{
    m_renderer->setCapture(m_awesomeCapture->isChecked());
}

void RendererDebugWidget::onLightViewChanged()
{
    m_renderer->setRenderLightView(m_lightView->isChecked());
}


void RendererDebugWidget::onVolumetricObscuranceChanged()
{
    if (m_lineVO->isChecked())
    {
        m_renderer->loadConfiguration("shaders/configurations/defaultConfig.xml");
    }
    else if(m_varianceVO->isChecked())
    {
        m_renderer->loadConfiguration("shaders/configurations/VarianceVO.xml");
    }
    else if(m_momentVO->isChecked())
    {
        m_renderer->loadConfiguration("shaders/configurations/MomentVO.xml");
    }
}

void RendererDebugWidget::onFilterShadowMapChanged()
{
    m_renderer->setFilterShadowMap(m_filterShadowMap->isChecked());
}

void RendererDebugWidget::onShadowMapMsaaChanged()
{
    for (int i = 0; i < 5; i++)
    {
        if (m_shadowMapMsaa[i]->isChecked())
        {
            m_renderer->setShadowMapSampleCount(1<<i);
        }
    }
}
