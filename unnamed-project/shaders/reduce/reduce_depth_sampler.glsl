#version 450

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding = 0) uniform sampler2D inputSampler;
layout (binding=1, rg16) writeonly uniform image1D outputTex;

uniform vec2 inputSize;

shared vec2 sharedData[256];

layout (local_size_x = 16, local_size_y = 16) in;

void updateMinMaxDepth(inout vec2 depthMinMax, in float depth)
{
    if (depth < 1)
    {
        depthMinMax.x = min(depthMinMax.x, depth);
        depthMinMax.y = max(depthMinMax.y, depth);
    }
}

void computeCurrentThreadValue(out vec2 depthMinMax)
{
    // d = 1 -> Background
    depthMinMax = vec2(1, 0);

    // TODO size via uniform!
    ivec2 inputPos = ivec2(gl_GlobalInvocationID.xy)*2;

    float depth;

    depth = texelFetch(inputSampler, inputPos, 0).x;
    updateMinMaxDepth(depthMinMax, depth);

    depth = texelFetch(inputSampler, inputPos + ivec2(1, 0), 0).x;
    updateMinMaxDepth(depthMinMax, depth);

    depth = texelFetch(inputSampler, inputPos + ivec2(0, 1), 0).x;
    updateMinMaxDepth(depthMinMax, depth);

    depth = texelFetch(inputSampler, inputPos + ivec2(1, 1), 0).x;
    updateMinMaxDepth(depthMinMax, depth);
}

void main()
{
    vec2 depthMinMax;
    computeCurrentThreadValue(depthMinMax);

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*16;

    depthMinMax.y = 1 - depthMinMax.y;
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
        int outputPos = int(gl_WorkGroupID.x + gl_WorkGroupID.y*gl_NumWorkGroups.x);
        imageStore(outputTex, outputPos, vec4(depthMinMax, 0, 0));
    }
}
