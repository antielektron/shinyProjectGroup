#version 430

// TODO in first iteration input is single channel!
// layout (binding=0, r16) readonly uniform image2D inputTex;
layout (binding = 0) uniform sampler2D inputSampler;
layout (binding=1, rgba8) writeonly uniform image2D outputTex;

uniform mat4 inverseProjectionMatrix;

layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec2 outputPos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 outputSize = imageSize(outputTex);

    ivec2 inputPos = 2 * outputPos;
    /*
    vec2 a = imageLoad(inputTex, inputPos);
    vec2 b = imageLoad(inputTex, inputPos + ivec(0, 1));
    vec2 c = imageLoad(inputTex, inputPos + ivec(1, 0));
    vec2 d = imageLoad(inputTex, inputPos + ivec(1, 1));
    */

    vec2 normalizedPos = vec2(outputPos) / vec2(outputSize);
    float d = texture2D(inputSampler, normalizedPos).x;

    vec4 screenValue = vec4(normalizedPos, d, 1.);
    vec4 worldValue = inverseProjectionMatrix * screenValue;
    worldValue /= -worldValue.w;

    imageStore(outputTex, outputPos, vec4(0., worldValue.z/50., 0., 1.));
}
