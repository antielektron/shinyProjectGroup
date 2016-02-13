#version 430

layout (binding=0, rg16) readonly uniform image1D inputTex;
layout (binding=1, rg16) writeonly uniform image1D outputTex;

layout (local_size_x = 256) in;

shared vec2 sharedData[256];

void computeCurrentThreadValue(out vec2 depthMinMax)
{
    int inputSize = imageSize(inputTex).x;
    int inputPos = 4 * int(gl_GlobalInvocationID.x);

    depthMinMax = vec2(1, 1);

    if (inputPos < inputSize)
    {
        vec2 value = imageLoad(inputTex, inputPos).xy;
        depthMinMax = min(depthMinMax, value);
    }

    inputPos++;

    if (inputPos < inputSize)
    {
        vec2 value = imageLoad(inputTex, inputPos).xy;
        depthMinMax = min(depthMinMax, value);
    }

    inputPos++;

    if (inputPos < inputSize)
    {
        vec2 value = imageLoad(inputTex, inputPos).xy;
        depthMinMax = min(depthMinMax, value);
    }

    inputPos++;

    if (inputPos < inputSize)
    {
        vec2 value = imageLoad(inputTex, inputPos).xy;
        depthMinMax = min(depthMinMax, value);
    }
}

void main()
{
    vec2 depthMinMax;
    computeCurrentThreadValue(depthMinMax);

    uint index = gl_LocalInvocationID.x;

    sharedData[index] = depthMinMax;

    if (index >= 128)
    {
        return;
    }

    vec2 other;

    // At least 32 threads per warp on modern gpu's

    barrier();

    other = sharedData[index + 128];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    barrier();

    other = sharedData[index + 64];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    barrier();

    other = sharedData[index + 32];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    barrier();

    other = sharedData[index + 16];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;


    other = sharedData[index + 8];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;


    other = sharedData[index + 4];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;


    other = sharedData[index + 2];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;


    other = sharedData[index + 1];
    depthMinMax = min(depthMinMax, other);
    // sharedData[index] = depthMinMax;


    if (index == 0)
    {
        int outputPos = int(gl_WorkGroupID.x);
        imageStore(outputTex, outputPos, vec4(depthMinMax, 0, 0));
    }
}
