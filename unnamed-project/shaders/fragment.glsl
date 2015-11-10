#version 130

in vec3 normal;

uniform vec4 color;

out vec4 outputColor;

void main()
{
    outputColor = vec4(normalize(normal),1.); // color;
}
