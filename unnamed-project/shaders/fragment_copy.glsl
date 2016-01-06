#version 150

in vec2 uv;

uniform sampler2D sampler;

out vec4 outputColor;

void main()
{
    outputColor = texture2D(sampler, uv);
}
