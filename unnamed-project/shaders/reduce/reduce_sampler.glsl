#version 430

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding = 0) uniform sampler2D inputSampler;
layout (binding=1, rg16) writeonly uniform image2D outputTex;

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

    // Access pixels at center
    vec2 normalizedInputPos = (vec2(inputPos) + 0.5) / inputSize;
    vec2 inputDelta = 1 / inputSize;

    float a = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxDepth(depthMinMax, a);

    float b = texture2D(inputSampler, normalizedInputPos + vec2(inputDelta.x, 0)).x;
    updateMinMaxDepth(depthMinMax, b);

    float c = texture2D(inputSampler, normalizedInputPos + vec2(0, inputDelta.y)).x;
    updateMinMaxDepth(depthMinMax, c);

    float d = texture2D(inputSampler, normalizedInputPos + inputDelta).x;
    updateMinMaxDepth(depthMinMax, d);
}

void main()
{
    vec2 depthMinMax;
    computeCurrentThreadValue(depthMinMax);

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*16;
    sharedData[index] = depthMinMax;

    barrier();

    vec2 other;

    // At least 32 threads per wrap on modern gpu's
    // if (index < 32)
    if (index < 128)
    {
        other = sharedData[index + 128];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        barrier();

        other = sharedData[index + 64];
        depthMinMax.x = min(depthMinMax.x, other.x);
        depthMinMax.y = max(depthMinMax.y, other.y);
        sharedData[index] = depthMinMax;

        barrier();

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
