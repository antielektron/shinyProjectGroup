#include <fstream>

#include "utility.h"

std::string loadTextFile(const std::string &fileName)
{
    std::ifstream input(fileName);
    std::istreambuf_iterator<char> eos;
    return std::string(std::istreambuf_iterator<char>(input), eos);
}


#ifdef HAVE_OPENAL

void playAudioSampleFromFile(ALbyte *wavfile)
{

    ALuint helloBuffer, helloSource;
    helloBuffer = alutCreateBufferHelloWorld();
    alGenSources(1, &helloSource);
    alSourcei(helloSource, AL_BUFFER, helloBuffer);
    alSourcePlay(helloSource);
    alutSleep(1);

}

#endif // HAVE_OPENAL
