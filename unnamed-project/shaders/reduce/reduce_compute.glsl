#version 430

layout (binding=0, rg16) readonly uniform image2D inputTex;
layout (binding=1, rg16) writeonly uniform image2D outputTex;

layout (local_size_x = 8, local_size_y = 8) in;

shared vec2 sharedData[64];

void updateMinMaxDepth(inout vec2 depthMinMax, in vec2 depth)
{
    depthMinMax.x = min(depthMinMax.x, depth.x);
    depthMinMax.y = max(depthMinMax.y, depth.y);
}

void computeCurrentThreadValue(out vec2 depthMinMax)
{
    ivec2 inputSize = imageSize(inputTex);
    ivec2 inputPos = 2 * ivec2(gl_GlobalInvocationID.xy);

    // d = 1 -> Background
    depthMinMax = vec2(1, 0);

    if (inputPos.x < inputSize.x && inputPos.y < inputSize.y)
    {
        vec2 a = imageLoad(inputTex, inputPos).xy;
        updateMinMaxDepth(depthMinMax, a);
    }

    if (inputPos.x+1 < inputSize.x && inputPos.y < inputSize.y)
    {
        vec2 b = imageLoad(inputTex, inputPos + ivec2(1, 0)).xy;
        updateMinMaxDepth(depthMinMax, b);
    }
    if (inputPos.x < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        vec2 c = imageLoad(inputTex, inputPos + ivec2(0, 1)).xy;
        updateMinMaxDepth(depthMinMax, c);
    }
    if (inputPos.x+1 < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        vec2 d = imageLoad(inputTex, inputPos + ivec2(1, 1)).xy;
        updateMinMaxDepth(depthMinMax, d);
    }
}

void main()
{
    vec2 depthMinMax;
    computeCurrentThreadValue(depthMinMax);

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*8;
    sharedData[index] = depthMinMax;

    barrier();

    vec2 other;

    // At least 32 threads per wrap on modern gpu's
    if (index < 32)
    {
        other = sharedData[index + 32];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        barrier();

        other = sharedData[index + 16];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        barrier();

        other = sharedData[index + 8];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        barrier();

        other = sharedData[index + 4];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        other = sharedData[index + 2];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        other = sharedData[index + 1];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;
    }

    if (index == 0)
    {
        ivec2 outputPos = ivec2(gl_WorkGroupID.xy);
        imageStore(outputTex, outputPos, vec4(depthMinMax, 0, 0));
    }
}
