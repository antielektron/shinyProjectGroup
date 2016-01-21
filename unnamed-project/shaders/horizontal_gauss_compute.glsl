#version 430

layout (binding=0, rgba8) readonly uniform image2D sourceImage;
layout (binding=1, rgba8) writeonly uniform image2D filteredImage;

const int kernelSize = 9;
const float weights[kernelSize] = {
    0.000002, 0.000428, 0.022321, 0.229742, 0.495015, 0.229742, 0.022321, 0.000428, 0.000002
};


layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(sourceImage);

    vec4 result = vec4(0, 0, 0, 0);

    for (int i = 0; i < kernelSize; i++)
    {
        vec4 value = imageLoad(sourceImage, pos + ivec2(i - 4, 0));
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}
