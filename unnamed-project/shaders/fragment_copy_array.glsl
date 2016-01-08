#version 150
#extension GL_EXT_texture_array : enable

in vec2 uv;

uniform int layer;

uniform sampler2DArray sampler;

out vec4 outputColor;

void main()
{
    outputColor = texture2DArray(sampler, vec3(uv, layer));
}
