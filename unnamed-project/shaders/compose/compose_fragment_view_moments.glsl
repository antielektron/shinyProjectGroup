#version 150

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D momentsSampler;
uniform sampler2D depthBuffer;
//uniform float time;

out vec4 outputColor;

float cascadeFar = 50;

float get_depth(vec2 uv)
{
	// workaround till i know why there are 
	// such strange values in the z-buffer
	return texture2D(sampler, uv).a;
}



void main()
{

	// temporary solution for demo:
	// get depth
	float z_1 = textureLod(momentsSampler, uv, 0).x;
	float z_2 = textureLod(momentsSampler, uv, 0).y;
	float z_3 = textureLod(momentsSampler, uv, 0).z;
	float z_4 = textureLod(momentsSampler, uv, 0).w;
 
	outputColor =  vec4(z_1,0, 0, 1.);
}
