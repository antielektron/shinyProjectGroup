#version 450

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding=0) uniform sampler2D inputSampler;
layout (binding=1, rgba16) writeonly uniform image1DArray outputTex;

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

    // depth = texture2D(inputSampler, normalizedInputPos).x;
    depth = texelFetch(inputSampler, inputPos, 0).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);

    normalizedInputPos.x += inputDelta.x;
    inputPos.x++;

    depth = texelFetch(inputSampler, inputPos, 0).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);

    normalizedInputPos.y += inputDelta.y;
    inputPos.y++;

    depth = texelFetch(inputSampler, inputPos, 0).x;
    updateMinMaxCorners(minCorners, maxCorners, normalizedInputPos, depth);

    normalizedInputPos.x -= inputDelta.x;
    inputPos.x--;

    depth = texelFetch(inputSampler, inputPos, 0).x;
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

    vec4 currentValue0;
    vec4 currentValue1;
    vec4 currentValue2;

    currentValue0 = sharedMinMax[index] = vec4(minCorners[0].x, minCorners[1].x, minCorners[2].x, minCorners[3].x);
    currentValue1 = sharedMinMax[index+256] = vec4(minCorners[0].y, minCorners[1].y, minCorners[2].y, minCorners[3].y);
    currentValue2 = sharedMinMax[index+512] = vec4(minCorners[0].z, minCorners[1].z, minCorners[2].z, minCorners[3].z);

    // transform into texture space [0, 1]
    maxCorners[0] = maxCorners[0] * 0.5 + 0.5;
    maxCorners[1] = maxCorners[1] * 0.5 + 0.5;
    maxCorners[2] = maxCorners[2] * 0.5 + 0.5;
    maxCorners[3] = maxCorners[3] * 0.5 + 0.5;

    sharedMinMax[index+768] = vec4(1) - vec4(maxCorners[0].x, maxCorners[1].x, maxCorners[2].x, maxCorners[3].x);
    sharedMinMax[index+1024] = vec4(1) - vec4(maxCorners[0].y, maxCorners[1].y, maxCorners[2].y, maxCorners[3].y);
    sharedMinMax[index+1280] = vec4(1) - vec4(maxCorners[0].z, maxCorners[1].z, maxCorners[2].z, maxCorners[3].z);

    barrier();

    vec4 other;

    // reduce different type of values on different threads!
    if (localIndex >= 128)
    {
        index += 768-128;
        localIndex -= 128;
        currentValue0 = sharedMinMax[index];
        currentValue1 = sharedMinMax[index + 256];
        currentValue2 = sharedMinMax[index + 512];
    }

    barrier();

    // At least 32 threads per warp on modern gpu's

    other = sharedMinMax[index + 128];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    other = sharedMinMax[index + 256 + 128];
    currentValue1 = min(currentValue1, other);
    sharedMinMax[index + 256] = currentValue1;

    other = sharedMinMax[index + 512 + 128];
    currentValue2 = min(currentValue2, other);
    sharedMinMax[index + 512] = currentValue2;

    barrier();

    other = sharedMinMax[index + 64];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    other = sharedMinMax[index + 256 + 64];
    currentValue1 = min(currentValue1, other);
    sharedMinMax[index + 256] = currentValue1;

    other = sharedMinMax[index + 512 + 64];
    currentValue2 = min(currentValue2, other);
    sharedMinMax[index + 512] = currentValue2;

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
        currentValue0 = sharedMinMax[index];
        // 4th thread group is dismissed!
    }

    if (localIndex >= 32)
    {
        index += 256-32;
        localIndex -= 32;
        currentValue0 = sharedMinMax[index];
    }

    barrier();

    other = sharedMinMax[index + 32];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    barrier();

    other = sharedMinMax[index + 16];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    barrier();

    other = sharedMinMax[index + 8];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    barrier();

    other = sharedMinMax[index + 4];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    barrier();

    other = sharedMinMax[index + 2];
    currentValue0 = min(currentValue0, other);
    sharedMinMax[index] = currentValue0;

    barrier();

    other = sharedMinMax[index + 1];
    currentValue0 = min(currentValue0, other);
    // sharedMinMax[index] = currentValue0;


    int outputPos = int(gl_WorkGroupID.x + gl_WorkGroupID.y*gl_NumWorkGroups.x);

    // used local variables currentMin
    if (originalIndex == 0)
    {
        // min X
        imageStore(outputTex, ivec2(outputPos, 0), currentValue0);
    }
    if (originalIndex == 32)
    {
        // min Y
        imageStore(outputTex, ivec2(outputPos, 1), currentValue0);
    }
    if (originalIndex == 64)
    {
        // min Z
        imageStore(outputTex, ivec2(outputPos, 2), currentValue0);
    }
    if (originalIndex == 128)
    {
        // max X
        imageStore(outputTex, ivec2(outputPos, 3), currentValue0);
    }
    if (originalIndex == 128 + 32)
    {
        // max Y
        imageStore(outputTex, ivec2(outputPos, 4), currentValue0);
    }
    if (originalIndex == 128 + 64)
    {
        // max Z
        imageStore(outputTex, ivec2(outputPos, 5), currentValue0);
    }
}
