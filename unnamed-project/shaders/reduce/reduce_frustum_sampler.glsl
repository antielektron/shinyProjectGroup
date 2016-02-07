#version 450

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding = 0) uniform sampler2D inputSampler;

layout (binding=1, rgba16) writeonly uniform image2D outputMinX;
layout (binding=2, rgba16) writeonly uniform image2D outputMinY;
layout (binding=3, rgba16) writeonly uniform image2D outputMinZ;

layout (binding=4, rgba16) writeonly uniform image2D outputMaxX;
layout (binding=5, rgba16) writeonly uniform image2D outputMaxY;
layout (binding=6, rgba16) writeonly uniform image2D outputMaxZ;

uniform vec2 inputSize;

// in screen space!
uniform float cascadeFar[3];

// TODO incude a projection matrix, that wraps the entire view frustum? -> could have easy default min/max corners!
uniform mat4 screenToLightMatrix;


shared vec4 sharedMinMax[6*256];

layout (local_size_x = 16, local_size_y = 16) in;

int getCascade(float depth)
{
    if (depth <= cascadeFar[0])
        return 0;
    else if (depth <= cascadeFar[1])
        return 1;
    else if (depth <= cascadeFar[2])
        return 2;
    else
        return 3;
}

void updateMinMaxCorners(inout vec3 minCorners[4], inout vec3 maxCorners[4], in vec2 uv, in float depth)
{
    if (depth < 1)
    {
        int index = getCascade(depth);

        // project!

        // NOTE: input is in [0,1]^3
        vec4 pos = screenToLightMatrix * (vec4(uv, depth, 1) * 2 - 1);
        pos /= pos.w;

        minCorners[index] = min(minCorners[index], pos.xyz);
        maxCorners[index] = max(maxCorners[index], pos.xyz);
    }
}

void computeCurrentThreadValue(out vec3 minCorners[4], out vec3 maxCorners[4])
{
    // initialize with default values
    minCorners[0] = minCorners[1] = minCorners[2] = minCorners[3] = vec3( 1, 1, 1);
    maxCorners[0] = maxCorners[1] = maxCorners[2] = maxCorners[3] = vec3(-1,-1,-1);

    ivec2 inputPos = ivec2(gl_GlobalInvocationID.xy)*2;

    // Access pixels at center
    vec2 normalizedInputPos = (vec2(inputPos) + 0.5) / inputSize;
    vec2 inputDelta = 1 / inputSize;

    float depth;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);

    normalizedInputPos.x += inputDelta.x;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);

    normalizedInputPos.y += inputDelta.y;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);

    normalizedInputPos.x -= inputDelta.x;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);
}

void main()
{
    vec3 minCorners[4];
    vec3 maxCorners[4];
    computeCurrentThreadValue(minCorners, maxCorners);

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*16;
    uint originalIndex = index;
    uint localIndex = index;

    // transform into texture space [0, 1]
    minCorners[0] = minCorners[0] * 0.5 + 0.5;
    minCorners[1] = minCorners[1] * 0.5 + 0.5;
    minCorners[2] = minCorners[2] * 0.5 + 0.5;
    minCorners[3] = minCorners[3] * 0.5 + 0.5;

    vec4 currentMinX = sharedMinMax[index] = vec4(minCorners[0].x, minCorners[1].x, minCorners[2].x, minCorners[3].x);
    vec4 currentMinY = sharedMinMax[index+256] = vec4(minCorners[0].y, minCorners[1].y, minCorners[2].y, minCorners[3].y);
    vec4 currentMinZ = sharedMinMax[index+512] = vec4(minCorners[0].z, minCorners[1].z, minCorners[2].z, minCorners[3].z);

    // transform into texture space [0, 1]
    maxCorners[0] = maxCorners[0] * 0.5 + 0.5;
    maxCorners[1] = maxCorners[1] * 0.5 + 0.5;
    maxCorners[2] = maxCorners[2] * 0.5 + 0.5;
    maxCorners[3] = maxCorners[3] * 0.5 + 0.5;

    vec4 currentMaxX = sharedMinMax[index+768] = -vec4(maxCorners[0].x, maxCorners[1].x, maxCorners[2].x, maxCorners[3].x);
    vec4 currentMaxY = sharedMinMax[index+1024] = -vec4(maxCorners[0].y, maxCorners[1].y, maxCorners[2].y, maxCorners[3].y);
    vec4 currentMaxZ = sharedMinMax[index+1280] = -vec4(maxCorners[0].z, maxCorners[1].z, maxCorners[2].z, maxCorners[3].z);

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

    /*

    // transform into texture space [0, 1]
    minCorners[0] = minCorners[0] * 0.5 + 0.5;
    minCorners[1] = minCorners[1] * 0.5 + 0.5;
    minCorners[2] = minCorners[2] * 0.5 + 0.5;
    minCorners[3] = minCorners[3] * 0.5 + 0.5;

    vec4 currentMinX = vec4(minCorners[0].x, minCorners[1].x, minCorners[2].x, minCorners[3].x);
    vec4 currentMinY = vec4(minCorners[0].y, minCorners[1].y, minCorners[2].y, minCorners[3].y);
    vec4 currentMinZ = vec4(minCorners[0].z, minCorners[1].z, minCorners[2].z, minCorners[3].z);

    maxCorners[0] = maxCorners[0] * 0.5 + 0.5;
    maxCorners[1] = maxCorners[1] * 0.5 + 0.5;
    maxCorners[2] = maxCorners[2] * 0.5 + 0.5;
    maxCorners[3] = maxCorners[3] * 0.5 + 0.5;

    vec4 currentMaxX = vec4(maxCorners[0].x, maxCorners[1].x, maxCorners[2].x, maxCorners[3].x);
    vec4 currentMaxY = vec4(maxCorners[0].y, maxCorners[1].y, maxCorners[2].y, maxCorners[3].y);
    vec4 currentMaxZ = vec4(maxCorners[0].z, maxCorners[1].z, maxCorners[2].z, maxCorners[3].z);



    ivec2 outputPos = ivec2(gl_GlobalInvocationID.xy);

    imageStore(outputMinX, outputPos, currentMinX);
    imageStore(outputMinY, outputPos, currentMinY);
    imageStore(outputMinZ, outputPos, currentMinZ);

    imageStore(outputMaxX, outputPos, currentMaxX);
    imageStore(outputMaxY, outputPos, currentMaxY);
    imageStore(outputMaxZ, outputPos, currentMaxZ);

    */
}
