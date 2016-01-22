#include "RendererBase.h"
#include <QDomElement>
#include <QDomDocument>
#include <iostream>

#define DOM_KEY_SHADER_TYPE "type"
#define DOM_KEY_SHADER_PROG_NAME "program"
#define DOM_KEY_SHADER_FILE "path"

//------------------------------------------------------------------------------
RendererBase::RendererBase()
{
    m_renderingPaused = false;
    m_singleFrameRenderingRequested = false;
}

//------------------------------------------------------------------------------
void RendererBase::saveConfiguration(const std::string &filename)
{
    // TODO
}

//------------------------------------------------------------------------------
void RendererBase::loadConfiguration(const std::string &filename)
{
    // TODO implemntation!
}

//------------------------------------------------------------------------------
void RendererBase::render(GLuint fbo, Scene *scene)
{
    if (m_renderingPaused && !m_singleFrameRenderingRequested)
    {
        return;
    }
    if (m_singleFrameRenderingRequested)
    {
        m_singleFrameRenderingRequested = false;
    }
    onRenderingInternal(fbo, scene);
}

//------------------------------------------------------------------------------
void RendererBase::setShaderSource(const std::string &shaderSrc,
                               const std::string &progName,
                               QOpenGLShader::ShaderTypeBit type)
{
    m_sources[std::make_pair(progName, type)] = shaderSrc;
}

//------------------------------------------------------------------------------
ShaderErrorType RendererBase::createProgram(const std::string &program)
{
    // check whether there are sources for given program
    auto vertexShaderIt = m_sources.find(
                              std::make_pair(program, QOpenGLShader::Vertex));
    auto fragmentShaderIt = m_sources.find(
                                std::make_pair(program, QOpenGLShader::Fragment));
    auto geometryShaderIt = m_sources.find(
                                std::make_pair(program, QOpenGLShader::Geometry));

    if (vertexShaderIt == m_sources.end() || fragmentShaderIt == m_sources.end())
    {
        return ShaderErrorType::MissingSourcesError;
    }

    // create new Program
    std::unique_ptr<QOpenGLShaderProgram> prog(new QOpenGLShaderProgram());

    // append cached vertex and fragment shader code
    if (!prog->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                       vertexShaderIt->second.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
        return ShaderErrorType::VertexShaderError;
    }
    if (!prog->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       fragmentShaderIt->second.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
        return ShaderErrorType::FragmentShaderError;
    }

    // append cached geometry shader, if there is one
    if (geometryShaderIt != m_sources.end())
    {
        if (!prog->addShaderFromSourceCode(QOpenGLShader::Geometry,
                                           geometryShaderIt->second.c_str()))
        {
            std::cerr << "could not load geometry shader" << std::endl;
            return ShaderErrorType::GeometryShaderError;
        }
    }

    // bind cached attribute locations
    for( const auto &v : m_attribLocs[program])
    {
        prog->bindAttributeLocation(v.second, v.first);
    }

    // link program
    if (!prog->link())
    {
        std::cerr << "could not link shader program" << std::endl;
        return ShaderErrorType::LinkingError;
    }

    // no errors so far, finally move program to our programs map
    QOpenGLShaderProgram *pProg = prog.get();
    m_programs[program] = std::move(prog);

    // get uniform locations:
    pProg->bind();
    for (const auto &v : m_uniformLocs[program])
    {
        *(v.first) = pProg->uniformLocation(v.second);
    }
    pProg->release();
    return ShaderErrorType::NoError;
}

//------------------------------------------------------------------------------
void RendererBase::getPrograms(std::vector<std::string> &progs)
{
    // just in case...
    progs.clear();
    for (const auto &pair : m_programs)
    {
        progs.push_back(pair.first);
    }
}

//------------------------------------------------------------------------------
void RendererBase::getShadersForProgram(const std::string &progName,
                                    ShaderSourcesType &shaders)
{
    for (const auto &pair : m_sources)
    {
        if (pair.first.first == progName)
        {
            shaders.push_back(std::make_pair(pair.first.second, pair.second));
            // ^ don't get confused!
        }
    }
}

//------------------------------------------------------------------------------
void RendererBase::resumeRendering()
{
    m_renderingPaused = false;
}

//------------------------------------------------------------------------------
void RendererBase::pauseRendering()
{
    m_renderingPaused = true;
}

//------------------------------------------------------------------------------
void RendererBase::requestSingleFrameRendering()
{
    m_singleFrameRenderingRequested = true;
}


