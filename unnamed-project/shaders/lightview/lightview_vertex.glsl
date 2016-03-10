#version 150

in vec3 v_position;
in vec3 v_normal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

out vec3 normal;

void main()
{
    vec4 pos = modelViewMatrix * vec4(v_position, 1.);
    gl_Position = projectionMatrix * modelViewMatrix * vec4(v_position, 1.);
    normal = (modelViewMatrix * vec4(v_normal, 0.)).xyz;
}
