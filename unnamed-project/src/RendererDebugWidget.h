#ifndef UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H
#define UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H

#include <QWidget>

#include <QRadioButton>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QButtonGroup>

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
    void onLightSheeringChanged();
    void onCascadesStrategyChanged();
    void onAwesomeCaptureCapture();
    void onAwesomeCaptureChanged();
    void onAwesomeSlotChanged(int slot);
    void onLightViewChanged();

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

    QSlider *m_cascadedShadowMapsLambda;

    QCheckBox *m_lightSheering;

    QPushButton *m_awesomeCapture;
    QCheckBox *m_awesomeCaptureEnabled;
    QRadioButton *m_awesomeCaptureSlot[4];
    QButtonGroup *m_awesomeSlotGroup;

    QCheckBox *m_lightView;

    QCheckBox *m_filterShadowMap;

    QRadioButton *m_shadowMapMsaa[5];

    QRadioButton *m_noVO;
    QRadioButton *m_lineVO;
    QRadioButton *m_varianceVO;
    QRadioButton *m_momentVO;
};

#endif // UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H
