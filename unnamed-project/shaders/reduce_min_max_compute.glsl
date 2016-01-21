#version 430

// TODO in first iteration input is single channel!
layout (binding=0, rg16f) readonly uniform image2D inputTex;
layout (binding=1, rg16f) writeonly uniform image2D outputTex;

layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec2 outputPos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 inputSize = imageSize(inputTex);

    ivec2 inputPos = 2*outputPos;

    vec2 a = imageLoad(inputTex, inputPos);
    vec2 b = imageLoad(inputTex, inputPos + ivec(0, 1));
    vec2 c = imageLoad(inputTex, inputPos + ivec(1, 0));
    vec2 d = imageLoad(inputTex, inputPos + ivec(1, 1));

    imageStore(outputTex, pos, vec4(min, max, 0., 0.));
}
