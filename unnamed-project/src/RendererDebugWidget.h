#ifndef UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H
#define UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H

#include <QWidget>

#include <QRadioButton>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QButtonGroup>
#include <QLabel>
class Renderer;

class RendererDebugWidget : public QWidget
{
    Q_OBJECT
public:
    RendererDebugWidget(Renderer *renderer, QWidget *parent = nullptr);
    ~RendererDebugWidget();

signals:
    void shaderChanged();

protected slots:
    void onColorCascadesChanged();
    void onCascadedShadowMapsLambdaChanged();
    void onLightShearingChanged();
    void onCascadesStrategyChanged();
    void onAwesomeCaptureCapture();
    void onAwesomeCaptureChanged();
    void onAwesomeSlotChanged(int slot);
    void onLightViewChanged();
    void onMomentViewChanged();

    void onVolumetricObscuranceChanged();
    void onSampleSliderChanged(int);
    void onPlainObscuranceChanged(int);
    void onSkyChanged(int);
    void onCursorChanged(int);
    void onMipMapStrategyChanged();

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

    QCheckBox *m_lightShearing;

    QPushButton *m_awesomeCapture;
    QCheckBox *m_awesomeCaptureEnabled;
    QRadioButton *m_awesomeCaptureSlot[4];
    QButtonGroup *m_awesomeSlotGroup;

    QCheckBox *m_lightView;

    QCheckBox *m_colorizeMoments;
    QCheckBox *m_filterShadowMap;
    QRadioButton *m_shadowMapMsaa[5];
    QCheckBox *m_momentView;

    QCheckBox *m_plainVO;
    QCheckBox *m_sky;
    QRadioButton *m_noVO;
    QRadioButton *m_lineVO;
    QCheckBox *m_GL_LINEAR;
    QCheckBox *m_MIPMAP_LINEAR;

    QSlider *m_sampleSlider;
    QLabel *m_sampleLabel;

    QRadioButton *m_varianceVO;
    QRadioButton *m_momentVO;

    QCheckBox *m_drawCursor;
};

#endif // UNNAMED_PROJECT_RENDERER_DEBUG_WIDGET_H
