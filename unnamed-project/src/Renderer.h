#ifndef UNNAMED_PROJECT_RENDERER_H
#define UNNAMED_PROJECT_RENDERER_H

#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <memory>
#include <array>
#include <map>
#include <unordered_map>
#include <string>

#include "RendererBase.h"
#include "Scene/Scene.h"

#define KEYSTR_PROGRAM_RENDER                   "Render"
#define KEYSTR_PROGRAM_RENDER_DEPTH             "Render Depth"
#define KEYSTR_PROGRAM_COMPOSE                  "Compose"
#define KEYSTR_PROGRAM_SHADOW                   "Shadow"
#define KEYSTR_PROGRAM_COPY                     "Copy"
#define KEYSTR_PROGRAM_REDUCE_DEPTH             "Reduce Depth"
#define KEYSTR_PROGRAM_REDUCE_DEPTH_SAMPLER     "Reduce Depth Sampler"
#define KEYSTR_PROGRAM_REDUCE_FRUSTUM           "Reduce Frustum"
#define KEYSTR_PROGRAM_REDUCE_FRUSTUM_SAMPLER   "Reduce Frustum Sampler"
#define KEYSTR_PROGRAM_HORIZONTAL_GAUSS         "Horizontal Gauss"
#define KEYSTR_PROGRAM_VERTICAL_GAUSS           "Vertical Gauss"
#define KEYSTR_PROGRAM_CREATE_MOMENTS           "Create Moments"

#define KEYSTR_PROGRAM_CREATE_CASCADE_FARS      "Create Cascade Fars"
#define KEYSTR_PROGRAM_CREATE_CASCADE_VIEWS     "Create Cascade Views"
#define KEYSTR_PROGRAM_CREATE_CASCADES_CPU      "Create Cascades CPU"

#define KEYSTR_PROGRAM_CREATE_CAPTURE           "Create Capture"
#define KEYSTR_PROGRAM_RENDER_CAPTURE           "Render Capture"
#define KEYSTR_PROGRAM_RENDER_LIGHTVIEW         "Render Light View"
#define KEYSTR_PROGRAM_RENDER_VISIBLE_SAMPLES   "Render Visible Samples"

#define NUM_VO_MIPMAP_LEVELS 10 //so there's a height of 1 pixel @toplevel for full-HD

class Renderer : public RendererBase
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void initialize() override;
    virtual void resize(int width, int height) override;

    virtual QWidget *createDebugWidget(QWidget *parent) override;

    enum CascadeStrategy
    {
        CascadedShadowMaps,
        SampleDistributionShadowMaps
    };

    void setColorizeCascades(bool enabled);
    bool getColorizeCascades();

    void setCascadedShadowMapsLambda(float lambda);
    float getCascadedShadowMapsLambda();

    void setLightShearing(bool enabled);
    bool getLightShearing();

    void setCascadeStrategy(CascadeStrategy strategy);
    CascadeStrategy getCascadeStrategy();

    void setShadowMapSampleCount(int sampleCount);
    int getShadowMapSampleCount();

    void setFilterShadowMap(bool enabled);
    bool getFilterShadowMap();

    void setSamples(int s);
    int getSamples();

    void setPlainObscurance(bool b);
    bool isPlainObscurance();

    void setSky(bool b);
    bool isSky();

    void setCursor(bool b);
    bool isCursor();

    void setShinyFilter(bool b);
    bool isShinyFilter();

    void awesomeCapture();

    void setCaptureSlot(int slot);
    int getCaptureSlot();

    void setCapture(bool enabled);
    bool getCapture();

    void setRenderLightView(bool enabled);

    void setRenderMomentView(bool enabled);

    void setMipMapStrategy(bool glLin, bool mmLin);
    void getMipMapStrategy(bool &glLine, bool &mmLin);

protected:
    virtual void onRenderingInternal(GLuint fbo, Scene *scene) override;
    void rotateVectorToVector(const QVector3D &source,
                              const QVector3D &destination,
                              QMatrix4x4 &matrix);
    void createLightViewMatrix(const QVector3D &lightDir, const QMatrix4x4 &inverseCameraView, QMatrix4x4 &matrix);
    void createFrustumProjectionMatrix(const QMatrix4x4 & frustumTransformation, QMatrix4x4 &matrix);

    void createShadowMap();

    void renderCapture(const QMatrix4x4 &cameraView, const QMatrix4x4 &cameraProjection);

    // Options
    bool m_colorizeCascades;
    float m_cascadedShadowMapsLambda;
    bool m_lightShearing;
    CascadeStrategy m_cascadeStrategy;

    int m_shadowMapSampleCount;
    bool m_filterShadowMap;

    // set if we need to recreate the shadow map..
    bool m_recreateShadowMap;

    //compose stuff:
    int m_samples;
    int m_plainObscurance;
    int m_sky;
    int m_shinyFilter;
    bool m_glLinear;
    bool m_mmLinear;
    int m_cursor;


    /// CAPTURED information
    bool m_captureRequested;
    bool m_captureEnabled;
    int m_captureSlot;
    GLuint m_capturedFrustumToWorldBuffer[4];

    bool m_renderLightView;
    bool m_renderMomentView;


    // TODO shadowMapSize as option
    // (TODO cascade couunt as option)

    GLsizei m_width;
    GLsizei m_height;

    GLsizei m_shadowMapSize;
    GLsizei m_cascades;

    GLuint m_cascadeFarBuffer;
    GLuint m_cascadeViewBuffer;

    // Render Shader
    int m_modelViewMatrixLoc;
    int m_projectionMatrixLoc;
    int m_cascadeViewMatrixLoc;
    int m_cascadeFarLoc;
    int m_lightDirectionLoc;
    int m_lightColorLoc;
    int m_specularColorLoc;
    int m_diffuseColorLoc;
    int m_ambientColorLoc;
    int m_shadowMapSamplerLoc;
    int m_colorizeCascadesLoc;

    // Render Depth Shader
    int m_depthOnlyModelViewMatrixLoc;
    int m_depthOnlyProjectionMatrixLoc;
    int m_depthOnlyInverseProjectionMatrixLoc;
    int m_depthOnlySceneDepth;

    // ShadowMap Shader
    int m_shadowMapWorldMatrixLoc;
    int m_shadowMapCascadeViewMatrixLoc;

    GLuint m_shadowMapFrameBuffer;
    GLuint m_shadowMapTexture;
    GLuint m_shadowMapTexture2;
    GLuint m_shadowMapDepthBuffer;

    // Create ShadowMap Moments
    int m_createMomentsSampleCountLoc;

    // Compose Shader
    int m_composeSamplerLoc;
    int m_composeMomentsSamplerLoc;
    int m_composeMomentsSampler2Loc;
    int m_composeProjectionMatrixLoc;
    int m_composeInverseProjectionMatrixLoc;

    int m_composeDepthBufferLoc;
    int m_ratioLoc;
    int m_composeLightDirectionLoc;

    int m_composeSamplesLoc;
    int m_composePlainObscuranceLoc;

    int m_composeSkyLoc;
    int m_composeShinyFilterLoc;
    int m_composeCursorLoc;

    // Copy Shader for texture arrays
    int m_copyArraySamplerLoc;
    int m_copyArrayLayerLoc;

    // Reduce
    // int m_reduceInverseProjectionMatrixLoc;
    int m_reduceDepthInputSizeLoc;
    int m_reduceFrustumInputSizeLoc;
    int m_reduceFrustumCascadeFarLoc;
    int m_reduceFrustumScreenToLightMatrixLoc;
    std::vector<GLuint> m_depthReduceTextures;
    std::vector<GLuint> m_frustumReduceTextureArrays;
    GLsizei m_reduceLastTextureSize;

    int m_createCascadeFarsCameraProjectionLoc;
    int m_createCascadeFarsInverseCameraProjectionLoc;
    int m_createCascadeFarsLambdaLoc;

    int m_createCascadeViewsInvTempProjMatrixLoc;
    int m_createCascadeViewsLightViewMatrixLoc;

    int m_createCascadesCpuCascadeFarLoc;
    int m_createCascadesCpuCascadeViewMatrixLoc;
    float m_ratio;


    // Capture
    int m_createCaptureInverseCameraProjectionLoc;
    int m_createCaptureInverseCameraViewLoc;
    int m_renderCaptureCascadeIndexLoc;
    int m_renderCaptureCameraProjectionLoc;
    int m_renderCaptureCameraViewLoc;
    int m_renderCaptureColorLoc;

    // LightView
    int m_lightViewModelViewMatrixLoc;
    int m_lightViewProjectionMatrixLoc;
    int m_lightViewDiffuseColorLoc;
    int m_lightViewAmbientColorLoc;
    int m_visibleSamplesScreenToLightLoc;


    QOpenGLVertexArrayObject m_quadVao;
    QOpenGLBuffer m_quadVbo;

    GLuint m_renderFrameBuffer;
    GLuint m_renderTexture;
    GLuint m_voMomentsTexture;
    GLuint m_voMomentsTexture2;
    GLuint m_renderDepthBuffer;
};

#endif // UNNAMED_PROJECT_RENDERER_H
