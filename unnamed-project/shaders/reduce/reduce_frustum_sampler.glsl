#version 450

// Depth buffer can not be bound as image2D for whatever reason.
layout (binding = 0) uniform sampler2D inputSampler;

layout (binding=1, rgba16) writeonly uniform image2D outputMin;
layout (binding=2, rgba16) writeonly uniform image2D outputMax;

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

void updateMinMaxCorners(inout vec3 minCorner, inout vec3 maxCorner, in vec2 uv, in float depth)
{
    if (depth < 1)
    {
        // project!

        // NOTE: input is in [0,1]^3
        vec4 pos = screenToLightMatrix * (vec4(uv, depth, 1) * 2 - 1);
        pos /= pos.w;

        minCorner = min(minCorner, pos.xyz);
        maxCorner = max(maxCorner, pos.xyz);
    }
}

void computeCurrentThreadValue(out vec3 minCorner, out vec3 maxCorner)
{
    // initialize with default values
    minCorner = vec3( 1, 1, 1);
    maxCorner = vec3(-1,-1,-1);

    ivec2 inputPos = ivec2(gl_GlobalInvocationID.xy)*2;

    // Access pixels at center
    vec2 normalizedInputPos = (vec2(inputPos) + 0.5) / inputSize;
    vec2 inputDelta = 1 / inputSize;

    float depth;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorner, maxCorner, normalizedInputPos, depth);

    normalizedInputPos.x += inputDelta.x;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorner, maxCorner, normalizedInputPos, depth);

    normalizedInputPos.y += inputDelta.y;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorner, maxCorner, normalizedInputPos, depth);

    normalizedInputPos.x -= inputDelta.x;

    depth = texture2D(inputSampler, normalizedInputPos).x;
    updateMinMaxCorners(minCorner, maxCorner, normalizedInputPos, depth);
}

void main()
{
    vec3 minCorner;
    vec3 maxCorner;
    computeCurrentThreadValue(minCorner, maxCorner);

    // transform into texture space [0, 1]
    minCorner = minCorner * 0.5 + 0.5;
    maxCorner = maxCorner * 0.5 + 0.5;

    ivec2 outputPos = ivec2(gl_GlobalInvocationID.xy);

    imageStore(outputMin, outputPos, vec4(minCorner, 1));
    imageStore(outputMax, outputPos, vec4(maxCorner, 1));
/*

    uint index = gl_LocalInvocationID.x + gl_LocalInvocationID.y*16;

    vec4 currentMinX, currentMinY, currentMinZ, currentMaxX, currentMaxY, currentMaxZ;

    currentMinX = sharedMinX[index] = vec4(minCorners[0].x, minCorners[1].x, minCorners[2].x, minCorners[3].x);
    currentMinY = sharedMinY[index] = vec4(minCorners[0].y, minCorners[1].y, minCorners[2].y, minCorners[3].y);
    currentMinZ = sharedMinZ[index] = vec4(minCorners[0].z, minCorners[1].z, minCorners[2].z, minCorners[3].z);

    // transform into texture space [0, 1]
    maxCorners[0] = maxCorners[0] * 0.5 + 0.5;
    maxCorners[1] = maxCorners[1] * 0.5 + 0.5;
    maxCorners[2] = maxCorners[2] * 0.5 + 0.5;
    maxCorners[3] = maxCorners[3] * 0.5 + 0.5;

    currentMaxX = sharedMaxX[index] = vec4(maxCorners[0].x, maxCorners[1].x, maxCorners[2].x, maxCorners[3].x);
    currentMaxY = sharedMaxY[index] = vec4(maxCorners[0].y, maxCorners[1].y, maxCorners[2].y, maxCorners[3].y);
    currentMaxZ = sharedMaxZ[index] = vec4(maxCorners[0].z, maxCorners[1].z, maxCorners[2].z, maxCorners[3].z);

    barrier();

    vec4 other;

    // TODO reduce different type of values on different threads!

    // At least 32 threads per wrap on modern gpu's
    // if (index < 32)
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
    */
}
