#ifndef UNNAMED_PROJECT_UTILITY_H
#define UNNAMED_PROJECT_UTILITY_H

#ifdef HAVE_OPENAL

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <QMessageBox>

#endif // HAVE_OPENAL

#include <string>

enum class ShaderErrorType {
    NoError,
    MissingSourcesError,
    VertexShaderError,
    FragmentShaderError,
    GeometryShaderError,
    LinkingError,
    UndefinedError //when this happens, little cute kittens are dying...
};

std::string loadTextFile(const std::string &fileName);


#ifdef HAVE_OPENAL

void playAudioSampleFromFile(ALbyte *wavfile);

#endif // HAVE_OPENAL

#endif // UNNAMED_PROJECT_UTILITY_H
