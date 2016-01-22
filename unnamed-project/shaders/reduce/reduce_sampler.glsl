#version 430

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding = 0) uniform sampler2D inputSampler;
layout (binding=1, rg16) writeonly uniform image2D outputTex;

layout (local_size_x = 8, local_size_y = 8) in;

void updateMinMaxDepth(inout vec2 depthMinMax, in float depth)
{
    if (depth < 1)
    {
        depthMinMax.x = min(depthMinMax.x, depth);
        depthMinMax.y = max(depthMinMax.y, depth);
    }
}

void main()
{
    ivec2 outputPos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 outputSize = imageSize(outputTex);

    // d = 1 -> Background
    vec2 depthMinMax = vec2(1, 0);

    // Access pixels at center
    vec2 normalizedInputPos = (vec2(2*outputPos) + 0.5) / vec2(outputSize);
    vec2 inputDelta = 0.5 / vec2(outputSize);

    float a = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxDepth(depthMinMax, a);

    float b = texture2D(inputSampler, normalizedInputPos + vec2(inputDelta.x, 0)).x;
    updateMinMaxDepth(depthMinMax, b);

    float c = texture2D(inputSampler, normalizedInputPos + vec2(0, inputDelta.y)).x;
    updateMinMaxDepth(depthMinMax, c);

    float d = texture2D(inputSampler, normalizedInputPos + inputDelta).x;
    updateMinMaxDepth(depthMinMax, d);

    imageStore(outputTex, outputPos, vec4(depthMinMax, 0, 0));
}
