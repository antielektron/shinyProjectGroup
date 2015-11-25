#ifndef UNNAMED_PROJECT_UTILITY_H
#define UNNAMED_PROJECT_UTILITY_H

#include <string>

enum class ShaderErrorType {
    NoError,
    VertexShaderError,
    FragmentShaderError,
    LinkingError,
    UndefinedError //when this happens, little cute kittens are dying...
};

std::string loadTextFile(const std::string &fileName);

#endif // UNNAMED_PROJECT_UTILITY_H
