#include "RendererBase.h"
#include <QDomElement>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QFile>
#include <iostream>
#include <stdexcept>

#define DOM_KEY_SHADER_TYPE "type"
#define DOM_KEY_SHADER_PROG_NAME "program"
#define DOM_KEY_SHADER_FILE "path"
#define DOM_KEY_SHADER_CONFIG_ROOT "shader_config"
#define DOM_KEY_SHADER "shader"

//------------------------------------------------------------------------------
RendererBase::RendererBase()
{
    m_renderingPaused = false;
    m_singleFrameRenderingRequested = false;
}

//------------------------------------------------------------------------------
void RendererBase::saveConfiguration(const std::string &filename)
{
    //DAMN... now we have Qt dependencies in our RendererBase :(
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly))
    {
        throw std::runtime_error("Could not write file '" + filename+ "'");
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement(DOM_KEY_SHADER_CONFIG_ROOT);

    // write shader configs
    for (const auto &entry : m_sources)
    {
        std::string filepath = getFilepath(entry.first.first, entry.first.second);
        if (filepath.length() > 0)
        {
            xmlWriter.writeStartElement(DOM_KEY_SHADER);
            xmlWriter.writeAttribute(DOM_KEY_SHADER_PROG_NAME,
                                     QString::fromStdString(entry.first.first));
            xmlWriter.writeAttribute(DOM_KEY_SHADER_TYPE,
                                     QString::fromStdString(
                                         shaderTypeToString.at(entry.first.second)));
            xmlWriter.writeAttribute(DOM_KEY_SHADER_FILE,
                                     QString::fromStdString(filepath));
            xmlWriter.writeEndElement();
        }
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
}

//------------------------------------------------------------------------------
void RendererBase::loadConfiguration(const std::string &filename)
{
    // reset:
    m_programs.clear();

    QDomDocument document;
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Could not open file '" + filename+ "'");
    }
    document.setContent(&file);
    file.close();

    // Parse header
    QDomElement root = document.documentElement();
    QString rootType = root.tagName();              // should be 'ShaderConfig'

    // Parse xml body:
    for (auto currentElement = root.firstChildElement();
         !currentElement.isNull();
         currentElement = currentElement.nextSiblingElement())
    {
        QString tag = currentElement.tagName();
        std::cout << "TAG " << tag.toStdString() << std::endl;
        if (tag != DOM_KEY_SHADER)
        {
            std::cout << "WARNING: unexpected tag '"
                      << tag.toStdString()
                      << "' in shader configuration"
                      << std::endl;
        }
        else
        {
            QString filepath = currentElement.attribute(DOM_KEY_SHADER_FILE);
            QString program = currentElement.attribute(DOM_KEY_SHADER_PROG_NAME);
            QString typeStr = currentElement.attribute(DOM_KEY_SHADER_TYPE);

            auto type = stringToShaderType.at(typeStr.toStdString());

            // TODO: error handling
            std::string shaderSource = loadTextFile(filepath.toStdString());

            setShaderSource(shaderSource,
                            program.toStdString(),
                            type,
                            filepath.toStdString());

        }
    }

    // create Programs
    for (const auto &entry : m_sources)
    {
        createProgram(entry.first.first);
    }

};

//------------------------------------------------------------------------------
std::string RendererBase::getFilepath(const std::string &progName,
                                             QOpenGLShader::ShaderTypeBit type)
{
    auto pair = std::make_pair(progName, type);
    auto entry = m_filenames.find(pair);
    if (entry == m_filenames.end())
    {
        return "";
    }
    return entry->second;
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
                               QOpenGLShader::ShaderTypeBit type,
                               const std::string &filepath)
{
    m_sources[std::make_pair(progName, type)] = shaderSrc;
    m_filenames[std::make_pair(progName, type)] = filepath;
}

//------------------------------------------------------------------------------
void RendererBase::setShaderFilepath(const std::string &filepath,
                                     const std::string &progName,
                                     QOpenGLShader::ShaderTypeBit type)
{
    m_filenames[std::make_pair(progName, type)] = filepath;
}

//------------------------------------------------------------------------------
ShaderErrorType RendererBase::createProgram(const std::string &program)
{
    // check whether there are sources for given program
    auto vertexShaderIt = m_sources.find(std::make_pair(program, QOpenGLShader::Vertex));
    auto fragmentShaderIt = m_sources.find(std::make_pair(program, QOpenGLShader::Fragment));
    auto geometryShaderIt = m_sources.find(std::make_pair(program, QOpenGLShader::Geometry));
    auto computeShaderIt = m_sources.find(std::make_pair(program, QOpenGLShader::Compute));

    // create new Program
    std::unique_ptr<QOpenGLShaderProgram> prog(new QOpenGLShaderProgram());

    // append cached vertex and fragment shader code
    if (vertexShaderIt != m_sources.end() && !prog->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderIt->second.c_str()))
    {
        std::cerr << "could not load vertex shader" << std::endl;
        return ShaderErrorType::VertexShaderError;
    }

    // append cached geometry shader, if there is one
    if (geometryShaderIt != m_sources.end() && !prog->addShaderFromSourceCode(QOpenGLShader::Geometry, geometryShaderIt->second.c_str()))
    {
        std::cerr << "could not load geometry shader" << std::endl;
        return ShaderErrorType::GeometryShaderError;
    }

    if (fragmentShaderIt != m_sources.end() && !prog->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderIt->second.c_str()))
    {
        std::cerr << "could not load fragment shader" << std::endl;
        return ShaderErrorType::FragmentShaderError;
    }

    // set compute shader if there is one
    if (computeShaderIt != m_sources.end() && !prog->addShaderFromSourceCode(QOpenGLShader::Compute, computeShaderIt->second.c_str()))
    {
        std::cerr << "could not load compute shader" << std::endl;
        return ShaderErrorType::GeometryShaderError; // TODO
    }

    // bind cached attribute locations
    for(const auto &v : m_attribLocs[program])
    {
        prog->bindAttributeLocation(v.second, v.first);
    }

    // link program
    if (!prog->link())
    {
        std::cerr << "could not link shader program " << program << std::endl;
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


