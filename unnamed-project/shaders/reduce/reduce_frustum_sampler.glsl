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


// TODO single large array!
shared vec4 sharedMinX[256];
shared vec4 sharedMinY[256];
shared vec4 sharedMinZ[256];

shared vec4 sharedMaxX[256];
shared vec4 sharedMaxY[256];
shared vec4 sharedMaxZ[256];


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

    // transform into texture space [0, 1]
    minCorners[0] = minCorners[0] * 0.5 + 0.5;
    minCorners[1] = minCorners[1] * 0.5 + 0.5;
    minCorners[2] = minCorners[2] * 0.5 + 0.5;
    minCorners[3] = minCorners[3] * 0.5 + 0.5;

    vec4 currentMinX = sharedMinX[index] = vec4(minCorners[0].x, minCorners[1].x, minCorners[2].x, minCorners[3].x);
    vec4 currentMinY = sharedMinY[index] = vec4(minCorners[0].y, minCorners[1].y, minCorners[2].y, minCorners[3].y);
    vec4 currentMinZ = sharedMinZ[index] = vec4(minCorners[0].z, minCorners[1].z, minCorners[2].z, minCorners[3].z);

    // transform into texture space [0, 1]
    maxCorners[0] = maxCorners[0] * 0.5 + 0.5;
    maxCorners[1] = maxCorners[1] * 0.5 + 0.5;
    maxCorners[2] = maxCorners[2] * 0.5 + 0.5;
    maxCorners[3] = maxCorners[3] * 0.5 + 0.5;

    vec4 currentMaxX = sharedMaxX[index] = vec4(maxCorners[0].x, maxCorners[1].x, maxCorners[2].x, maxCorners[3].x);
    vec4 currentMaxY = sharedMaxY[index] = vec4(maxCorners[0].y, maxCorners[1].y, maxCorners[2].y, maxCorners[3].y);
    vec4 currentMaxZ = sharedMaxZ[index] = vec4(maxCorners[0].z, maxCorners[1].z, maxCorners[2].z, maxCorners[3].z);

    barrier();

    vec4 other;

    // TODO reduce different type of values on different threads!

    // At least 32 threads per wrap on modern gpu's
    if (index < 128)
    {
        other = sharedMinX[index + 128];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 128];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 128];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 128];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 128];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 128];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;

        barrier();

        other = sharedMinX[index + 64];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 64];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 64];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 64];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 64];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 64];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;

        barrier();

        other = sharedMinX[index + 32];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 32];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 32];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 32];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 32];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 32];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;

        barrier();

        other = sharedMinX[index + 16];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 16];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 16];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 16];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 16];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 16];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;

        barrier();

        other = sharedMinX[index + 8];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 8];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 8];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 8];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 8];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 8];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;


        other = sharedMinX[index + 4];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 4];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 4];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 4];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 4];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 4];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;


        other = sharedMinX[index + 2];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 2];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 2];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 2];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 2];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 2];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;


        other = sharedMinX[index + 1];
        currentMinX = min(currentMinX, other);
        sharedMinX[index] = currentMinX;

        other = sharedMinY[index + 1];
        currentMinY = min(currentMinY, other);
        sharedMinY[index] = currentMinY;

        other = sharedMinZ[index + 1];
        currentMinZ = min(currentMinZ, other);
        sharedMinZ[index] = currentMinZ;

        other = sharedMaxX[index + 1];
        currentMaxX = max(currentMaxX, other);
        sharedMaxX[index] = currentMaxX;

        other = sharedMaxY[index + 1];
        currentMaxY = max(currentMaxY, other);
        sharedMaxY[index] = currentMaxY;

        other = sharedMaxZ[index + 1];
        currentMaxZ = max(currentMaxZ, other);
        sharedMaxZ[index] = currentMaxZ;
    }

    if (index == 0)
    {
        ivec2 outputPos = ivec2(gl_WorkGroupID.xy);

        // TODO write to all textures at once using multiple threads!!

        imageStore(outputMinX, outputPos, currentMinX);
        imageStore(outputMinY, outputPos, currentMinY);
        imageStore(outputMinZ, outputPos, currentMinZ);

        imageStore(outputMaxX, outputPos, currentMaxX);
        imageStore(outputMaxY, outputPos, currentMaxY);
        imageStore(outputMaxZ, outputPos, currentMaxZ);
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
