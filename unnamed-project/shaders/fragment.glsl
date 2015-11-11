#version 130

in vec3 normal;
in vec3 v_lightDirection;
in vec3 v_lightColor;

uniform vec4 color;

out vec4 outputColor;

void main()
{
    outputColor = vec4(normalize(normal*normal),1.); // color;
}
