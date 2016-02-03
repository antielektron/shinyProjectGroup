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
    helloBuffer = alutCreateBufferFromFile("sounds/imperial_march.wav");
    if (helloBuffer == -1)
    {
        QMessageBox::critical(nullptr, "ERROR", " could not load audio file");
    }
    alGenSources(1, &helloSource);
    alSourcei(helloSource, AL_BUFFER, helloBuffer);
    alSourcePlay(helloSource);
    alSource3f(helloSource, AL_POSITION, -1.0, 0.0, 0.0);
    for (int i = 0; i < 10; i++)
    {
        for (float j = -5.0; j < 5.0; j+= 0.5)
        {
            alSource3f(helloSource, AL_POSITION, j, 5.0, 0.0);
            alutSleep(0.1);
        }
        for (float j = 5.0; j > -5.0; j -= 0.5)
        {
            alSource3f(helloSource, AL_POSITION, j, 5.0, 0.0);
            alutSleep(0.1);
        }
    }
    alutSleep(1);

}

#endif // HAVE_OPENAL
