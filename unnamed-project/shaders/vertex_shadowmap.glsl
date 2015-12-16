#version 130

in vec3 v_position;
in vec3 v_normal; // unused..

uniform mat4 modelViewProjection;

void main()
{
    gl_Position = modelViewProjection * vec4(v_position, 1.);
}
