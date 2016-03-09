#ifndef UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H
#define UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H

#include <QWidget>

#include <QRadioButton>
#include <QSlider>
#include <QCheckBox>

class Renderer;

class RendererDebugWidget : public QWidget
{
    Q_OBJECT
public:
    RendererDebugWidget(Renderer *renderer, QWidget *parent = nullptr);
    ~RendererDebugWidget();

protected slots:
    void onColorCascadesChanged();
    void onCascadedShadowMapsLambdaChanged();
    void onCascadesStrategyChanged();
    void onAwesomeCaptureChanged();
    void onVolumetricObscuranceChanged();

    void onFilterShadowMapChanged();
    void onShadowMapMsaaChanged();

private:
    void generateWidgets();
    QWidget *generateSDSMOptions();
    QWidget *generateMSMOptions();
    QWidget *generateVOOptions();
    void connectStuff();

    Renderer *m_renderer;

    QCheckBox *m_colorizeCascades;

    QRadioButton *m_cascadedShadowMaps;
    QRadioButton *m_sampleDistributionShadowMaps;

    QRadioButton *m_lineVO;
    QRadioButton *m_varianceVO;
    QRadioButton *m_momentVO;

    QSlider *m_cascadedShadowMapsLambda;

    QCheckBox *m_awesomeCapture;

    QCheckBox *m_filterShadowMap;

    QRadioButton *m_shadowMapMsaa[5];
};

#endif // UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H
