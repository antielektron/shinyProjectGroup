#version 430

layout (binding = 0) readonly buffer cascadeViewBuffer
{
    mat4 cascadeViewMatrix[4];
};

layout (binding = 1) writeonly buffer frustumToWorldBuffer
{
    mat4 frustumToWorldMatrix[5];
};

uniform mat4 inverseCameraProjection;
uniform mat4 inverseCameraView;

layout (local_size_x = 4) in;

void main()
{
    int index = int(gl_GlobalInvocationID.x);

    frustumToWorldMatrix[index] = inverseCameraView * inverse(cascadeViewMatrix[index]);

    if (index == 0)
    {
        // Compute camera frustum inversion
        frustumToWorldMatrix[4] = inverseCameraView * inverseCameraProjection;
    }
}
