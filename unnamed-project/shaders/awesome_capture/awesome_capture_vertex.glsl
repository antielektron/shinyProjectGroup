#version 430

in vec3 v_corner;

uniform int cascadeIndex;

uniform mat4 cameraView;
uniform mat4 cameraProjection;

layout (binding = 0) buffer frustumToWorldBuffer
{
    mat4 frustumToWorldMatrix[5];
};

void main()
{
    gl_Position = cameraProjection * cameraView * frustumToWorldMatrix[cascadeIndex] * vec4(v_corner, 1.);
}
