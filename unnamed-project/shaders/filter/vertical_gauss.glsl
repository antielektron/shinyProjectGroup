#version 430

layout (binding=0, rgba16) readonly uniform image2D sourceImage;
layout (binding=1, rgba16) writeonly uniform image2D filteredImage;

const int kernelSize = 9;
const float weights[kernelSize] = {
    0.000002, 0.000428, 0.022321, 0.229742, 0.495015, 0.229742, 0.022321, 0.000428, 0.000002
};

layout (local_size_x = 16, local_size_y = 16) in;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    vec4 result = vec4(0, 0, 0, 0);

    ivec2 curPos = pos + ivec2(0, -4);
    for (int i = 0; i < 9; i++)
    {
        vec4 value = imageLoad(sourceImage, curPos);
        curPos.y++;
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}

/*
    // work group size + 2 * blur radius
    shared vec4 cache[256 + 2*4];
*/

/*  256x1   600ms @ 1000x

    int startPos = int(gl_LocalInvocationID.y);

    // read two pixels..
    cache[startPos + 4] = imageLoad(sourceImage, pos);

    if (startPos < 4)
    {
        cache[startPos] = imageLoad(sourceImage, pos - ivec2(0, 4));
    }
    if (startPos >= 256-4)
    {
        cache[startPos+8] = imageLoad(sourceImage, pos + ivec2(0, 4));
    }

    barrier();

    vec4 result = vec4(0, 0, 0, 0);

    for (int i = 0; i < 9; i++)
    {
        vec4 value = cache[startPos + i];
        result += value * weights[i];
    }

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(filteredImage, pos, result);

*/

/*  8x1     1500ms @ 1000x

    int startPos = int(gl_LocalInvocationID.y);
    int readPos = startPos * 2;
    ivec2 sourcePos = ivec2(gl_GlobalInvocationID.x, gl_WorkGroupID.y * gl_WorkGroupSize.y + readPos - 4);

    // read two pixels..
    cache[readPos] = imageLoad(sourceImage, sourcePos);
    cache[readPos+1] = imageLoad(sourceImage, sourcePos + ivec2(0, 1));

    barrier();

    vec4 result = vec4(0, 0, 0, 0);

    for (int i = 0; i < 9; i++)
    {
        vec4 value = cache[startPos + i];
        result += value * weights[i];
    }

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(filteredImage, pos, result);

*/

/* 8x8      400ms @ 1000x

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    vec4 result = vec4(0, 0, 0, 0);

    for (int i = 0; i < 9; i++)
    {
        vec4 value = imageLoad(sourceImage, pos + ivec2(0, i - 4));
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);

*/
