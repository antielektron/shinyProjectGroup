#version 150

in vec3 v_position;
in vec3 v_normal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(v_position, 1.);
}
