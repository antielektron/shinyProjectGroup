#version 130

in vec3 v_position;
in vec3 v_normal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 lightViewMatrix;

out vec3 normal;
out vec3 worldPosition;
out vec4 lightViewPosition;

void main()
{
	vec4 pos = modelViewMatrix * vec4(v_position, 1.);
    gl_Position = projectionMatrix * pos;
    normal = (modelViewMatrix * vec4(v_normal, 0.)).xyz;
	worldPosition = pos.xyz;
    lightViewPosition = lightViewMatrix * vec4(v_position, 1.);
}
