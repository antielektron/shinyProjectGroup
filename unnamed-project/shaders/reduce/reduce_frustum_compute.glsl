#version 430

layout (binding=0, rgba16) readonly uniform image2D inputMinX;
layout (binding=1, rgba16) readonly uniform image2D inputMinY;
layout (binding=2, rgba16) readonly uniform image2D inputMinZ;

layout (binding=3, rgba16) readonly uniform image2D inputMaxX;
layout (binding=4, rgba16) readonly uniform image2D inputMaxY;
layout (binding=5, rgba16) readonly uniform image2D inputMaxZ;

layout (binding=6, rgba16) writeonly uniform image2D outputMinX;
layout (binding=7, rgba16) writeonly uniform image2D outputMinY;
layout (binding=8, rgba16) writeonly uniform image2D outputMinZ;

layout (binding=9, rgba16) writeonly uniform image2D outputMaxX;
layout (binding=10, rgba16) writeonly uniform image2D outputMaxY;
layout (binding=11, rgba16) writeonly uniform image2D outputMaxZ;


shared vec4 sharedMinMax[6*256];

layout (local_size_x = 16, local_size_y = 16) in;

void computeCurrentThreadValue(out vec4 minX, out vec4 minY, out vec4 minZ, out vec4 maxX, out vec4 maxY, out vec4 maxZ)
{
    ivec2 inputSize = imageSize(inputMinX);
    ivec2 inputPos = 2 * ivec2(gl_GlobalInvocationID.xy);

    // initialize with default values
    minX = minY = minZ = vec4(1);
    maxX = maxY = maxZ = vec4(0);

    if (inputPos.x < inputSize.x && inputPos.y < inputSize.y)
    {
        minX = min(imageLoad(inputMinX, inputPos), minX);
        minY = min(imageLoad(inputMinY, inputPos), minY);
        minZ = min(imageLoad(inputMinZ, inputPos), minZ);

        maxX = max(imageLoad(inputMaxX, inputPos), maxX);
        maxY = max(imageLoad(inputMaxY, inputPos), maxY);
        maxZ = max(imageLoad(inputMaxZ, inputPos), maxZ);
    }

    if (inputPos.x+1 < inputSize.x && inputPos.y < inputSize.y)
    {
        ivec2 pos = inputPos + ivec2(1, 0);

        minX = min(imageLoad(inputMinX, pos), minX);
        minY = min(imageLoad(inputMinY, pos), minY);
        minZ = min(imageLoad(inputMinZ, pos), minZ);

        maxX = max(imageLoad(inputMaxX, pos), maxX);
        maxY = max(imageLoad(inputMaxY, pos), maxY);
        maxZ = max(imageLoad(inputMaxZ, pos), maxZ);
    }
    if (inputPos.x < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        ivec2 pos = inputPos + ivec2(0, 1);

        minX = min(imageLoad(inputMinX, pos), minX);
        minY = min(imageLoad(inputMinY, pos), minY);
        minZ = min(imageLoad(inputMinZ, pos), minZ);

        maxX = max(imageLoad(inputMaxX, pos), maxX);
        maxY = max(imageLoad(inputMaxY, pos), maxY);
        maxZ = max(imageLoad(inputMaxZ, pos), maxZ);
    }
    if (inputPos.x+1 < inputSize.x && inputPos.y+1 < inputSize.y)
    {
        ivec2 pos = inputPos + ivec2(1, 1);

        minX = min(imageLoad(inputMinX, pos), minX);
        minY = min(imageLoad(inputMinY, pos), minY);
        minZ = min(imageLoad(inputMinZ, pos), minZ);

        maxX = max(imageLoad(inputMaxX, pos), maxX);
        maxY = max(imageLoad(inputMaxY, pos), maxY);
        maxZ = max(imageLoad(inputMaxZ, pos), maxZ);
    }
}

void main()
{
    vec4 currentMinX;
    vec4 currentMinY;
    vec4 currentMinZ;

    vec4 currentMaxX;
    vec4 currentMaxY;
    vec4 currentMaxZ;

    computeCurrentThreadValue(currentMinX, currentMinY, currentMinZ, currentMaxX, currentMaxY, currentMaxZ);

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*16;
    uint originalIndex = index;
    uint localIndex = index;

    sharedMinMax[index] = currentMinX;
    sharedMinMax[index+256] = currentMinY;
    sharedMinMax[index+512] = currentMinZ;

    sharedMinMax[index+768] = -currentMaxX;
    sharedMinMax[index+1024] = -currentMaxY;
    sharedMinMax[index+1280] = -currentMaxZ;

    barrier();

    vec4 other;

    // reduce different type of values on different threads!
    if (localIndex >= 128)
    {
        index += 768-128;
        localIndex -= 128;
        currentMinX = sharedMinMax[index];
        currentMinY = sharedMinMax[index + 256];
        currentMinZ = sharedMinMax[index + 512];
    }

    // At least 32 threads per warp on modern gpu's

    other = sharedMinMax[index + 128];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;

    other = sharedMinMax[index + 256 + 128];
    currentMinY = min(currentMinY, other);
    sharedMinMax[index + 256] = currentMinY;

    other = sharedMinMax[index + 512 + 128];
    currentMinZ = min(currentMinZ, other);
    sharedMinMax[index + 512] = currentMinZ;

    barrier();

    other = sharedMinMax[index + 64];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;

    other = sharedMinMax[index + 256 + 64];
    currentMinY = min(currentMinY, other);
    sharedMinMax[index + 256] = currentMinY;

    other = sharedMinMax[index + 512 + 64];
    currentMinZ = min(currentMinZ, other);
    sharedMinMax[index + 512] = currentMinZ;

    barrier();

    // dismiss 4th sub group..
    if (localIndex >= 64+32)
    {
        return;
    }

    if (localIndex >= 64)
    {
        index += 512-64;
        localIndex -= 64;
        currentMinX = sharedMinMax[index];
        // 4th thread group is dismissed!
    }

    if (localIndex >= 32)
    {
        index += 256-32;
        localIndex -= 32;
        currentMinX = sharedMinMax[index];
    }

    other = sharedMinMax[index + 32];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;



    other = sharedMinMax[index + 16];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;



    other = sharedMinMax[index + 8];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;



    other = sharedMinMax[index + 4];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;



    other = sharedMinMax[index + 2];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;



    other = sharedMinMax[index + 1];
    currentMinX = min(currentMinX, other);
    sharedMinMax[index] = currentMinX;


    ivec2 outputPos = ivec2(gl_WorkGroupID.xy);

    // used local variables currentMin
    if (originalIndex == 0)
    {
        imageStore(outputMinX, outputPos, currentMinX);
    }
    if (originalIndex == 32)
    {
        imageStore(outputMinY, outputPos, currentMinX);
    }
    if (originalIndex == 64)
    {
        imageStore(outputMinZ, outputPos, currentMinX);
    }
    if (originalIndex == 128)
    {
        imageStore(outputMaxX, outputPos, -currentMinX);
    }
    if (originalIndex == 128 + 32)
    {
        imageStore(outputMaxY, outputPos, -currentMinX);
    }
    if (originalIndex == 128 + 64)
    {
        imageStore(outputMaxZ, outputPos, -currentMinX);
    }
}
