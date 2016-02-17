#version 450

layout (binding = 0, rg16) readonly uniform image1D inputTex;

layout (binding = 1) writeonly buffer cascadeFarBuffer
{
    vec4 cascadeFar;
};

uniform mat4 cameraProjectionMatrix;
uniform mat4 inverseCameraProjectionMatrix;

// Coefficient for combining uniform and logarithmic results
const float lambdaUniLog = 0.3;

layout (local_size_x = 4) in;

void main()
{
    int index = int(gl_GlobalInvocationID.x);

    vec2 depthMinMax = imageLoad(inputTex, 0).xy;

    depthMinMax = depthMinMax * vec2(2, -2) + vec2(-1, 1);

    // Transform corners into light view space
    vec4 nearPoint = inverseCameraProjectionMatrix * vec4(0, 0, depthMinMax.x, 1);
    vec4 farPoint = inverseCameraProjectionMatrix * vec4(0, 0, depthMinMax.y, 1);

    // work with negative z's
    float nearPlane = nearPoint.z / nearPoint.w;
    float farPlane = farPoint.z / farPoint.w;

    // interpolate between actual near/far planes
    float farIndex = (index+1.)/4.; // name?
    float cUni = nearPlane + (farPlane - nearPlane) * farIndex;
    float cLog = nearPlane * pow(farPlane / nearPlane, farIndex);

    // combine cLog and cUni
    float currentZ = lambdaUniLog * cUni + (1. - lambdaUniLog) * cLog;

    // z in screen space!
    vec4 screenPos = cameraProjectionMatrix * vec4(0, 0, currentZ, 1);
    float screenZ = screenPos.z/screenPos.w*.5 + .5;

    cascadeFar[index] = screenZ;
}
