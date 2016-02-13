#version 450

layout (binding=0, rgba16) readonly uniform image1DArray inputTex;
layout (binding=1, rgba16) writeonly uniform image1DArray outputTex;


shared vec4 sharedMinMax[6*256];

layout (local_size_x = 256) in;

void computeCurrentThreadValue(out vec4 value0, out vec4 value1, out vec4 value2, out vec4 value3, out vec4 value4, out vec4 value5)
{
    int inputSize = imageSize(inputTex).x;
    int inputPos = 4 * int(gl_GlobalInvocationID.x); // TODO find good factor!

    // initialize with default values
    value0 = value1 = value2 = value3 = value4 = value5 = vec4(1, 1, 1, 1);

    if (inputPos < inputSize)
    {
        value0 = min(imageLoad(inputTex, ivec2(inputPos, 0)), value0);
        value1 = min(imageLoad(inputTex, ivec2(inputPos, 1)), value1);
        value2 = min(imageLoad(inputTex, ivec2(inputPos, 2)), value2);

        value3 = min(imageLoad(inputTex, ivec2(inputPos, 3)), value3);
        value4 = min(imageLoad(inputTex, ivec2(inputPos, 4)), value4);
        value5 = min(imageLoad(inputTex, ivec2(inputPos, 5)), value5);
    }

    inputPos++;

    if (inputPos < inputSize)
    {
        value0 = min(imageLoad(inputTex, ivec2(inputPos, 0)), value0);
        value1 = min(imageLoad(inputTex, ivec2(inputPos, 1)), value1);
        value2 = min(imageLoad(inputTex, ivec2(inputPos, 2)), value2);

        value3 = min(imageLoad(inputTex, ivec2(inputPos, 3)), value3);
        value4 = min(imageLoad(inputTex, ivec2(inputPos, 4)), value4);
        value5 = min(imageLoad(inputTex, ivec2(inputPos, 5)), value5);
    }

    inputPos++;

    if (inputPos < inputSize)
    {
        value0 = min(imageLoad(inputTex, ivec2(inputPos, 0)), value0);
        value1 = min(imageLoad(inputTex, ivec2(inputPos, 1)), value1);
        value2 = min(imageLoad(inputTex, ivec2(inputPos, 2)), value2);

        value3 = min(imageLoad(inputTex, ivec2(inputPos, 3)), value3);
        value4 = min(imageLoad(inputTex, ivec2(inputPos, 4)), value4);
        value5 = min(imageLoad(inputTex, ivec2(inputPos, 5)), value5);
    }

    inputPos++;

    if (inputPos < inputSize)
    {
        value0 = min(imageLoad(inputTex, ivec2(inputPos, 0)), value0);
        value1 = min(imageLoad(inputTex, ivec2(inputPos, 1)), value1);
        value2 = min(imageLoad(inputTex, ivec2(inputPos, 2)), value2);

        value3 = min(imageLoad(inputTex, ivec2(inputPos, 3)), value3);
        value4 = min(imageLoad(inputTex, ivec2(inputPos, 4)), value4);
        value5 = min(imageLoad(inputTex, ivec2(inputPos, 5)), value5);
    }
}

void main()
{
    vec4 currentValue0;
    vec4 currentValue1;
    vec4 currentValue2;
    vec4 currentValue3;
    vec4 currentValue4;
    vec4 currentValue5;

    computeCurrentThreadValue(currentValue0, currentValue1, currentValue2, currentValue3, currentValue4, currentValue5);

    uint index = gl_LocalInvocationID.x;
    uint originalIndex = index;
    uint localIndex = index;

    sharedMinMax[index] = currentValue0;
    sharedMinMax[index+256] = currentValue1;
    sharedMinMax[index+512] = currentValue2;

    sharedMinMax[index+768] = currentValue3;
    sharedMinMax[index+1024] = currentValue4;
    sharedMinMax[index+1280] = currentValue5;

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


    int outputPos = int(gl_WorkGroupID.x);

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
