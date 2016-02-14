#version 430

layout (binding=0, rgba16) readonly uniform image2D sourceImage;
// layout (binding=0) uniform sampler2D sourceSampler;
layout (binding=1, rgba16) writeonly uniform image2D filteredImage;

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

layout (local_size_x = 16, local_size_y = 16) in;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    vec4 result = vec4(0, 0, 0, 0);

    ivec2 curPos = pos + ivec2(-4, 0);
    for (int i = 0; i < 9; i++)
    {
        vec4 value = imageLoad(sourceImage, curPos);
        curPos.x++;
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}

/*

layout (local_size_x = 16, local_size_y = 16) in;

// 16 entries per row, 4 before, 8 in group, 4 after
shared vec4 sharedData[16 * 24];

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    int index = int(4 + gl_LocalInvocationID.x + gl_LocalInvocationID.y * 24);

    // TODO check for bank conflicts
    sharedData[index] = imageLoad(sourceImage, pos);

    int delta = gl_LocalInvocationID.x < 8 ? -4 : 4;
    sharedData[index + delta] = imageLoad(sourceImage, pos + ivec2(delta, 0));

    // no barrier required.
    // barrier();

    index -= 4;
    vec4 result = vec4(0, 0, 0, 0);
    for (int i = 0; i < 9; i++)
    {
        vec4 value = sharedData[index];
        index++;
        // vec2 normalizedPos = (vec2(pos) + vec2(0.5, 0.5) + vec2(offsetHW[i], 0.)) / vec2(size);
        // vec2 normalizedPos = (vec2(pos) + vec2(0.5, 0.5) + vec2(i - 4, 0.)) / vec2(size);
        // vec4 value = texture2D(sourceSampler, normalizedPos);
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}

*/
