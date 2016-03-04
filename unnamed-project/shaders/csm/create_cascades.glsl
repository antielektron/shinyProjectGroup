#version 430

layout (binding = 0) writeonly buffer cascadeFarBuffer
{
    vec4 cascadeFar;
};

layout (binding = 1) writeonly buffer cascadeViewBuffer
{
    mat4 cascadeViewMatrix[4];
};

// do work on cpu!
uniform vec4 inputCascadeFar;
uniform mat4 inputCascadeViewMatrix[4];

layout (local_size_x = 4) in;

void main()
{
    int index = int(gl_GlobalInvocationID.x);

    cascadeViewMatrix[index] = inputCascadeViewMatrix[index];

    if (index == 0)
        cascadeFar = inputCascadeFar;
}
