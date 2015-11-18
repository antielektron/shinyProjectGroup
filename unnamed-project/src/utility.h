#ifndef UNNAMED_PROJECT_UTILITY_H
#define UNNAMED_PROJECT_UTILITY_H

#include <string>

enum class OpenglErrorType {
    noError,
    vertexShaderError,
    fragmentShaderError,
    linkingError,
    undefinedError //when this happens, little cute kittens are dying...
};

std::string loadTextFile(const std::string &fileName);

#endif // UNNAMED_PROJECT_UTILITY_H
