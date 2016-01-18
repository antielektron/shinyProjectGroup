#version 150

in vec2 uv;
uniform mat4 projectionMatrix;

uniform sampler2D sampler;
uniform float time;

out vec4 outputColor;

const float PI = 3.1415926536;

// misc:

// stupid pseudo random generator:
vec2 rand_state = uv;//vec2(0.5,0.5);
float rand_internal(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand(float a)
{
	float res = rand_internal(rand_state);
	rand_state = vec2(fract(rand_state.x + res +a), fract(rand_state.y + res+a)); 
	return res;
}

// stuff for line sampling:

int samples = 10; //TODO, pass through shader

// very stupid perspective scaling approximation function:
float get_k(float z)
{
	// physically not correct
	vec4 e = vec4(1.0, 0., z, 1.);
	e = projectionMatrix * e;
	return e.x;
}

float get_dr(vec2 unitPos, vec2 pPos, float p_z)
{
	float k = get_k(p_z);
	vec2 scaledUnitPos = vec2(unitPos.x * k, unitPos.y * k);
	vec2 screenSpaceUnitPos = scaledUnitPos + pPos;
	//clip if necessary:
	/*
	if (screenSpaceUnitPos.x > 1.)
	{
	    screenSpaceUnitPos.x = 1.;
	}
	if (screenSpaceUnitPos.y > 1.)
	{
	    screenSpaceUnitPos.y = 1.;
	}
	if (screenSpaceUnitPos.x < 0.)
	{
	    screenSpaceUnitPos.x = 0.;
	}
	if (screenSpaceUnitPos.y < 0.)
	{
	    screenSpaceUnitPos.y = 0.;
	}	
	*/
	
	return p_z - texture2D(sampler, screenSpaceUnitPos).a;
}

float z_s(vec2 unitPos)
{
    return sqrt(1 - unitPos.x * unitPos.x - unitPos.y * unitPos.y);
}

float sampleLineStep(vec2 unitPos, vec2 pPos, float p_z)
{
	float zs = z_s(unitPos);
	return max(min(zs, get_dr(unitPos, pPos, p_z)) + zs, 0) / (2. * zs); 	
}

float lineSampling(int nSamples)
{
	float sum = 0.;
	float z = texture2D(sampler, uv).a;
	for (int i = 0; i < nSamples; i++)
	{
		// get a random angle and radius for sample point on the
		// unit disk
		float radius = rand(z);
		float angle = rand(z) * 2 * PI;
		
		vec2 samplePoint = vec2(cos(angle) * radius,sin(angle) * radius);
		sum += sampleLineStep(samplePoint, uv, z);
	}
	return sum / float(nSamples);
}

void main()
{
	float z = (texture2D(sampler, uv).a);
	float obscuranceFactor = lineSampling(samples);
	vec4 color = vec4(texture2D(sampler, uv).xyz , 2 * obscuranceFactor);
    outputColor = color;
}
