#include <fstream>

#include "utility.h"

std::string loadTextFile(const std::string &fileName)
{
    std::ifstream input(fileName);
    std::istreambuf_iterator<char> eos;
    return std::string(std::istreambuf_iterator<char>(input), eos);
}
