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

    m_sampleSlider->setValue(m_renderer->getSamples());
    m_sampleLabel->setText(QString("Samples: " ) + QString::number(m_renderer->getSamples()));

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

    m_lineVO->setChecked(true);
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

    m_lightShearing = new QCheckBox("Light Shearing");
    layout->addWidget(m_lightShearing);

    m_cascadedShadowMaps = new QRadioButton("CSM", sdsmOptions);
    layout->addWidget(m_cascadedShadowMaps);

    m_sampleDistributionShadowMaps = new QRadioButton("SDSM", sdsmOptions);
    layout->addWidget(m_sampleDistributionShadowMaps);

    m_lightView = new QCheckBox("Light View");
    layout->addWidget(m_lightView);

    m_awesomeCapture = new QPushButton("Awesome Capture");
    layout->addWidget(m_awesomeCapture);

    m_awesomeCaptureEnabled = new QCheckBox("Enable Capture");
    layout->addWidget(m_awesomeCaptureEnabled);

    m_awesomeSlotGroup = new QButtonGroup(this);

    for (int i = 0; i < 4; i++)
    {
        m_awesomeCaptureSlot[i] = new QRadioButton(QString("Slot ") + QString::number(i));
        layout->addWidget(m_awesomeCaptureSlot[i]);
        m_awesomeSlotGroup->addButton(m_awesomeCaptureSlot[i], i);
    }

    return sdsmOptions;
}

QWidget *RendererDebugWidget::generateVOOptions()
{
    auto voOptions = new QGroupBox("VO variants", this);
    auto layout = new QVBoxLayout(voOptions);
    voOptions->setLayout(layout);

    m_lineVO = new QRadioButton("line sampling", voOptions);
    layout->addWidget(m_lineVO);

    m_sampleSlider = new QSlider(Qt::Horizontal, voOptions);
    layout->addWidget(m_sampleSlider);
    m_sampleSlider->setRange(0,1000);
    m_sampleSlider->setTickInterval(1);

    m_sampleLabel = new QLabel("", voOptions);
    layout->addWidget(m_sampleLabel);

    m_varianceVO = new QRadioButton("variance area sampling", voOptions);
    layout->addWidget(m_varianceVO);

    m_momentVO = new QRadioButton("moment area sampling", voOptions);
    layout->addWidget(m_momentVO);

    m_noVO = new QRadioButton("plain copy", voOptions);
    layout->addWidget(m_noVO);

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

    m_momentView = new QCheckBox("Moment View", this);
    layout->addWidget(m_momentView);

    return msmOptions;
}

void RendererDebugWidget::connectStuff()
{
    connect(m_colorizeCascades, SIGNAL(stateChanged(int)), this, SLOT(onColorCascadesChanged()));
    connect(m_cascadedShadowMapsLambda, SIGNAL(valueChanged(int)), this, SLOT(onCascadedShadowMapsLambdaChanged()));
    connect(m_lightShearing, SIGNAL(stateChanged(int)), this, SLOT(onLightShearingChanged()));
    connect(m_cascadedShadowMaps, SIGNAL(clicked()), this, SLOT(onCascadesStrategyChanged()));
    connect(m_sampleDistributionShadowMaps, SIGNAL(clicked()), this, SLOT(onCascadesStrategyChanged()));
    connect(m_lightView, SIGNAL(stateChanged(int)), this, SLOT(onLightViewChanged()));

    connect(m_awesomeCapture, SIGNAL(clicked(bool)), this, SLOT(onAwesomeCaptureCapture()));
    connect(m_awesomeCaptureEnabled, SIGNAL(stateChanged(int)), this, SLOT(onAwesomeCaptureChanged()));
    connect(m_awesomeSlotGroup, SIGNAL(buttonClicked(int)), this, SLOT(onAwesomeSlotChanged(int)));

    connect(m_lineVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));
    connect(m_sampleSlider, SIGNAL(valueChanged(int)), this, SLOT(onSampleSliderChanged(int)));
    connect(m_varianceVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));
    connect(m_momentVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));
    connect(m_noVO, SIGNAL(clicked()), this, SLOT(onVolumetricObscuranceChanged()));

    // connect(m_colorizeMoments, SIGNAL(stateChanged(int)), this, SLOT(onColorMomentsChanged));
    connect(m_filterShadowMap, SIGNAL(stateChanged(int)), this, SLOT(onFilterShadowMapChanged()));
    for (int i = 0; i < 5; i++)
    {
        connect(m_shadowMapMsaa[i], SIGNAL(clicked()), this, SLOT(onShadowMapMsaaChanged()));
    }
    connect(m_momentView, SIGNAL(stateChanged(int)), this, SLOT(onMomentViewChanged()));
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

void RendererDebugWidget::onLightShearingChanged()
{
    m_renderer->setLightShearing(m_lightShearing->isChecked());
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

void RendererDebugWidget::onAwesomeCaptureCapture()
{
    m_renderer->awesomeCapture();
}

void RendererDebugWidget::onAwesomeCaptureChanged()
{
    m_renderer->setCapture(m_awesomeCaptureEnabled->isChecked());
}

void RendererDebugWidget::onAwesomeSlotChanged(int slot)
{
    m_renderer->setCaptureSlot(slot);
}

void RendererDebugWidget::onLightViewChanged()
{
    m_renderer->setRenderLightView(m_lightView->isChecked());
}

void RendererDebugWidget::onMomentViewChanged()
{
    m_renderer->setRenderMomentView(m_momentView->isChecked());
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
    else if (m_noVO->isChecked())
    {
        m_renderer->loadConfiguration("shaders/configurations/plainConfig.xml");
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

void RendererDebugWidget::onSampleSliderChanged(int s)
{
    m_renderer->setSamples(s);
    m_sampleLabel->setText(QString("Samples: " ) + QString::number(s));
}
