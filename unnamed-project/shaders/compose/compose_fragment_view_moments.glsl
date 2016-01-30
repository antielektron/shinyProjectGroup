#version 150

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D momentsSampler;
uniform sampler2D depthBuffer;
//uniform float time;

out vec4 outputColor;



void main()
{
	vec4 moments = texture2D(momentsSampler, uv);
	float mean = moments.x;
	float variance = moments.y;
	
    outputColor = vec4(mean, variance, 0.0, 1.);
}
