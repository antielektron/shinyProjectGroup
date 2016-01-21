#version 430

layout (binding=0, rg16) readonly uniform image2D inputTex;
layout (binding=1, rg16) writeonly uniform image2D outputTex;

layout (local_size_x = 8, local_size_y = 8) in;

void updateMinMaxDepth(inout vec2 depthMinMax, in vec2 depth)
{
    depthMinMax.x = min(depthMinMax.x, depth.x);
    depthMinMax.y = max(depthMinMax.y, depth.y);
}

void main()
{
    ivec2 outputPos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 outputSize = imageSize(outputTex);

    ivec2 inputSize = imageSize(inputTex);
    ivec2 inputPos = 2 * outputPos;

    // d = 1 -> Background
    vec2 depthMinMax = vec2(1, 0);

    vec2 a = imageLoad(inputTex, inputPos).xy;
    updateMinMaxDepth(depthMinMax, a);

    if (inputPos.x+1 < inputSize.x)
    {
        vec2 b = imageLoad(inputTex, inputPos + ivec2(1, 0)).xy;
        updateMinMaxDepth(depthMinMax, b);
    }
    if (inputPos.y+1 < inputSize.y)
    {
        vec2 c = imageLoad(inputTex, inputPos + ivec2(0, 1)).xy;
        updateMinMaxDepth(depthMinMax, c);
    }
    if (inputPos.x+1 < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        vec2 d = imageLoad(inputTex, inputPos + ivec2(1, 1)).xy;
        updateMinMaxDepth(depthMinMax, d);
    }

    imageStore(outputTex, outputPos, vec4(depthMinMax, 0, 0));
}
