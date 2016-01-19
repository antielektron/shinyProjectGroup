#version 150

in vec2 uv;
uniform mat4 projectionMatrix;

uniform sampler2D sampler;
uniform float time;

out vec4 outputColor;

int samples = 10; //TODO, pass through shader
const float PI = 3.1415926536;
const float eps = 10e-8;

// factors to (linear) merge default shading
// and volumetric obscurance 
const float voShadingAmount = 0.5;
const float dfShadingAmount = 0.5;

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

// very stupid perspective scaling approximation function:
float get_k(float z)
{
	// physically not correct
	vec4 e = vec4(1.0, 0., z, 1.);
	e = projectionMatrix * e;
	return 1 / (z*10 + 4);
}

// weight function (calculates how much of the sphere is filled
// by a corresponding line sample
float get_w(float radius)
{
    // trivial solution, just calculate the line length:
    // (= pythagoras)
    return sqrt(1 - pow(radius,2));
}

float get_dr(vec2 unitPos, vec2 pPos, float p_z)
{
	float k = get_k(p_z);
	vec2 scaledUnitPos = vec2(unitPos.x * k, unitPos.y * k);
	vec2 screenSpaceUnitPos = scaledUnitPos + pPos;
	//clip if necessary:
	float max = 1. - eps;

	if (screenSpaceUnitPos.x > max)
	{
	    screenSpaceUnitPos.x = max;
	}
	if (screenSpaceUnitPos.y > max)
	{
	    screenSpaceUnitPos.y = max;
	}
	if (screenSpaceUnitPos.x < 0)
	{
	    screenSpaceUnitPos.x = 0;
	}
	if (screenSpaceUnitPos.y < 0.)
	{
	    screenSpaceUnitPos.y = 0.;
	}	
	
	
	return (- p_z + (1 - texture2D(sampler, screenSpaceUnitPos).a)) / k;
}

float z_s(vec2 unitPos)
{
    return sqrt(1 - unitPos.x * unitPos.x - unitPos.y * unitPos.y);
}

float sampleLineStep(vec2 unitPos, vec2 pPos, float p_z)
{
	float zs = z_s(unitPos);
	return max(min(zs, get_dr(unitPos, pPos, p_z)) + zs, 0) / (1 * zs); 	
}

float lineSampling(int nSamples)
{
	float sumSamples = 0.;
	float sumVolume = 0.;
	float z = 1 - texture2D(sampler, uv).a;
	for (int i = 0; i < nSamples; i++)
	{
		// get a random angle and radius for sample point on the
		// unit disk
		float radius = rand(z);
		float angle = rand(z) * 2 * PI;
		
		// weight samples:
		float w = get_w(radius);
		
		vec2 samplePoint = vec2(cos(angle) * radius,sin(angle) * radius);
		sumSamples += w * sampleLineStep(samplePoint, uv, z);
		sumVolume += w;
	}
	return sumSamples / sumVolume;
}


// debug stuff:
/*
bool isInCenterEpsilonArea(vec2 centerPoint)
{
	float zCenter = 1 - texture2D(sampler, centerPoint).a;
	float k = get_k(zCenter);
	
	float dx = uv.x - centerPoint.x;
	float dy = uv.y - centerPoint.y;
	
	return (dx * dx + dy * dy) < (k * k); 
}
*/

void main()
{
	vec3 defaultColor = texture2D(sampler, uv).xyz;
	vec3 voColor = lineSampling(samples) * vec3(1.,1.,1.);
	vec3 mixedColor = voShadingAmount * voColor
	                + dfShadingAmount * defaultColor;
	
	// DEBUG
	/*
	vec2 center = vec2(0.5,0.5);
	
	if (isInCenterEpsilonArea(center))
	{
		mixedColor.z *= 2 ;
		mixedColor.x *= 0.5;
		mixedColor.y *= 0.5;
	}
	
	if ( pow(uv.x - 0.5,2) < 10e-7)
	{
	    mixedColor.x = 1.;
		mixedColor.y = 0;
		mixedColor.z = 0;
	}
	if (pow(uv.y - 0.5,2) < 10e-7)
	{
		mixedColor.x = 1.;
		mixedColor.y = 0;
		mixedColor.z = 0;		
		
	}
	*/
	
    outputColor = vec4(mixedColor, 1.);
}
