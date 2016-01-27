#version 150

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D ovSampler;
uniform sampler2D depthBuffer;
//uniform float time;

out vec4 outputColor;

const int samples = 10; //TODO, pass through shader
const float PI = 3.1415926536;
const float eps = 10e-8;

// factors to (linear) merge default shading
// and volumetric obscurance 
const float voShadingAmount = 1.;
const float dfShadingAmount = 1.;

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


// improve depth scaling:
float get_world_depth(vec2 uv)
{
	float z = texture2D(depthBuffer, uv).x * 2.0 - 1.0;
	vec4 v = inverseProjectionMatrix * vec4(0.0,0.0, z, 1.0);	
	return 1.0 - v.z / (v.w);
}

float get_depth(vec2 uv)
{
	float z = texture2D(depthBuffer, uv).x * 2.0 - 1.0;
	vec4 v = inverseProjectionMatrix * vec4(0.0,0.0, z, 1.0);
	return  (v.z / ((v.w) * 100.0) + 1.0);
}


// stuff for line sampling:

// very stupid perspective scaling approximation function:
float get_kx(float z)
{
	// physically not correct
	vec4 e1 = vec4(1., 0., z, 1.);
	vec4 e2 = vec4(0., 0., z, 1.);
	e1 = projectionMatrix * e1;
	e2 = projectionMatrix * e2;
	
	//return 0.1;
	return 0.1 * (e2.x / e2.w - e1.x / e1.w);

}

float get_ky(float z)
{
	// physically not correct
	vec4 e1 = vec4(0., 1., z, 1.);
	vec4 e2 = vec4(0., 0., z, 1.);
	e1 = projectionMatrix * e1;
	e2 = projectionMatrix * e2;
	
	//return 0.1;
	return 0.1 * (e2.y / e2.w - e1.y / e1.w);	
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
	float kx = get_kx(get_world_depth(pPos));
	float ky = get_kx(get_world_depth(pPos));
	vec2 scaledUnitPos = vec2(unitPos.x * kx, unitPos.y * ky);
	vec2 screenSpaceUnitPos = scaledUnitPos + pPos;
	//clip if necessary:
	float max = 1. - eps;

	screenSpaceUnitPos = clamp(screenSpaceUnitPos, 0, 1 - eps);
	
	
	return (- p_z + (1 - get_depth(screenSpaceUnitPos))) / kx;
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
	float z = 1.0 - get_depth(uv);
	for (int i = 0; i < nSamples; i++)
	{
		// get a random angle and radius for sample point on the
		// unit disk
		float radius = rand(0.1);
		float angle = rand(0.1) * 2 * PI;
		
		// weight samples:
		float w = get_w(radius);
		
		vec2 samplePoint = vec2(cos(angle) * radius,sin(angle) * radius);
		sumSamples += w * sampleLineStep(samplePoint, uv, z);
		sumVolume += w;
	}
	return sumSamples / sumVolume;
}


// debug stuff:

bool isInCenterEpsilonArea(vec2 centerPoint)
{
	float zCenter = get_world_depth(centerPoint);
	float kx = get_kx(zCenter);
	float ky = get_ky(zCenter);
	
	// aspect ratio:
	float ratio = ky / kx;
	
	
	float dx = (uv.x - centerPoint.x) * ratio;
	float dy = (uv.y - centerPoint.y);
	
	return (dx * dx + dy * dy) < (kx * kx); 
}

//cursor
bool isCursor()
{

	float kx = get_kx(0.5);
	float ky = get_ky(0.5);
	
	// aspect ratio:
	float ratio = ky / kx;
	
	float dx = (uv.x - 0.5) * ratio;
	float dy = (uv.y - 0.5);
	float dx2 = dx * dx;
	float dy2 = dy * dy;
	
	if (dx2 < 0.0001)
	{
		return false;
	}
	
	if (dx2 + dy2 > 0.0005 && dx2 + dy2 < 0.001)
	{
		return true;
	}
	return false;
}


void main()
{
	vec3 defaultColor = dfShadingAmount * texture2D(sampler, uv).xyz;
	vec3 mixedColor = lineSampling(samples) * defaultColor;

	if (isCursor())
	{
		mixedColor = vec3(1.,1.,1.) - mixedColor;
	}
/*
	float depth = texture2D(ovSampler, uv).x;
	vec4 result = inverse(projectionMatrix) * vec4(0, 0, depth, 1);
	vec3 mixedColor = (result.z / -result.w) / 50. * vec3(1., 1., 1.);
*/
	// DEBUG
	
	vec2 center = vec2(0.5,0.5);
	
	if (isInCenterEpsilonArea(center))
	{
		mixedColor.z = 1. ;
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
	
	
    outputColor = vec4(mixedColor, 1.);
    
    // render just Depth:
    //float z = get_depth(uv);
    //outputColor = vec4(1 - z * vec3(1.,1.,1.), 1.);
}
