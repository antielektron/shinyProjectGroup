#version 130

in vec3 v_position;
in vec3 v_normal; // unused..

uniform mat4 lightViewMatrix;

out vec4 lightViewPosition;

void main()
{
    gl_Position = lightViewPosition = lightViewMatrix * vec4(v_position, 1.);
}
