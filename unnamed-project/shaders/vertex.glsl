#version 130

in vec3 v_position;
in vec3 v_normal;

uniform mat4 modelView;
uniform mat4 projection;

out vec3 normal;
out vec3 world;

void main()
{
	vec4 pos = modelView * vec4(v_position, 1.);
    gl_Position = projection * pos;
	world = pos.xyz;
    normal = (modelView * vec4(v_normal, 0.)).xyz;
}
