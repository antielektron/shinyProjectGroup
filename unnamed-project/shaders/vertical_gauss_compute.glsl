#version 430

layout (binding=0, rgba8) readonly uniform image2D sourceImage;
// layout (binding=0) uniform sampler2D sourceSampler;
layout (binding=1, rgba8) writeonly uniform image2D filteredImage;

const float weights[9] = {
    0.000002, 0.000428, 0.022321, 0.229742, 0.495015, 0.229742, 0.022321, 0.000428, 0.000002
};

const float offsetHW[5] = {
    -4.0 + 0.000428 / (0.000002 + 0.000428),
    -2.0 + 0.229742 / (0.022321 + 0.229742),
    0., // center
    1.0 + 0.022321 / (0.022321 + 0.229742),
    3.0 + 0.000002 / (0.000002 + 0.000428),
};

const float weightsHW[5] = {
    (0.000002 + 0.000428),
    (0.022321 + 0.229742),
    0.495015,
    (0.022321 + 0.229742),
    (0.000002 + 0.000428)
};

layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(filteredImage);

    vec4 result = vec4(0, 0, 0, 0);

    for (int i = 0; i < 9; i++)
    {
        vec4 value = imageLoad(sourceImage, pos + ivec2(0, i - 4));
        // vec2 normalizedPos = (vec2(pos) + vec2(0.5, 0.5) + vec2(0., offsetHW[i])) / vec2(size);
        // vec2 normalizedPos = (vec2(pos) + vec2(0.5, 0.5) + vec2(0., i - 4)) / vec2(size);
        // vec4 value = texture2D(sourceSampler, normalizedPos);
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}
