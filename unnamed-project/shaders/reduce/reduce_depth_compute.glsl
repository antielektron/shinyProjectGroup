#version 430

layout (binding=0, rg16) readonly uniform image2D inputTex;
layout (binding=1, rg16) writeonly uniform image2D outputTex;

layout (local_size_x = 16, local_size_y = 16) in;

shared vec2 sharedData[256];

void computeCurrentThreadValue(out vec2 depthMinMax)
{
    ivec2 inputSize = imageSize(inputTex);
    ivec2 inputPos = 2 * ivec2(gl_GlobalInvocationID.xy);

    depthMinMax = vec2(1);

    if (inputPos.x < inputSize.x && inputPos.y < inputSize.y)
    {
        vec2 value = imageLoad(inputTex, inputPos).xy;
        depthMinMax = min(depthMinMax, value);
    }

    if (inputPos.x+1 < inputSize.x && inputPos.y < inputSize.y)
    {
        vec2 value = imageLoad(inputTex, inputPos + ivec2(1, 0)).xy;
        depthMinMax = min(depthMinMax, value);
    }
    if (inputPos.x < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        vec2 value = imageLoad(inputTex, inputPos + ivec2(0, 1)).xy;
        depthMinMax = min(depthMinMax, value);
    }
    if (inputPos.x+1 < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        vec2 value = imageLoad(inputTex, inputPos + ivec2(1, 1)).xy;
        depthMinMax = min(depthMinMax, value);
    }
}

void main()
{
    vec2 depthMinMax;
    computeCurrentThreadValue(depthMinMax);

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*16;

    sharedData[index] = depthMinMax;

    barrier();

    if (index >= 128)
    {
        return;
    }

    vec2 other;

    // At least 32 threads per warp on modern gpu's

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

    barrier();

    other = sharedData[index + 8];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    barrier();

    other = sharedData[index + 4];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    other = sharedData[index + 2];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    other = sharedData[index + 1];
    depthMinMax = min(depthMinMax, other);
    sharedData[index] = depthMinMax;

    if (index == 0)
    {
        ivec2 outputPos = ivec2(gl_WorkGroupID.xy);
        imageStore(outputTex, outputPos, vec4(depthMinMax, 0, 0));
    }
}
