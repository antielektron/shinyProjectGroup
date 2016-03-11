#version 430

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding = 0) uniform sampler2D depthSampler;
layout (binding = 1, rgba8) writeonly uniform image2D outputTex;

uniform mat4 screenToLightTransformation;

layout (local_size_x = 16, local_size_y = 16) in;

void main()
{
    ivec2 imageSize = ivec2(imageSize(outputTex));

    ivec2 inputPos = ivec2(gl_GlobalInvocationID.xy);

    float depth = texelFetch(depthSampler, inputPos, 0).x;

    if (depth < 1) // 1 is faaar
    {
        vec3 texPos = vec3((inputPos + vec2(0.5))/vec2(imageSize), depth);
        vec4 screenPos = vec4(texPos * 2 - 1, 1);
        vec4 lightPos = screenToLightTransformation * screenPos;
        vec2 outputTexPos = (lightPos.xy/lightPos.w) * 0.5 + 0.5;
        imageStore(outputTex, ivec2(outputTexPos*imageSize), vec4(1, .5, 0, 1));
    }
}
