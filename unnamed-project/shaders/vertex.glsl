#version 130

in vec3 v_position;
in vec3 v_normal;

uniform mat4 modelView;
uniform mat4 projection;

out vec3 normal;

void main()
{
    gl_Position = projection * modelView * vec4(v_position, 1.);
    normal = (modelView * vec4(v_normal, 0.)).xyz;
}
