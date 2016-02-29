#version 430

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D momentsSampler;
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

// TODO: pass this values to shader
const float worldSpaceRadius = 5;
const float farPlane = 30;


//cursor
/*
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
*/

bool isInCenterEpsilonArea(float ssRadius)
{
	
	
	float dx = (uv.x - 0.5);
	float dy = (uv.y - 0.5);
	
	return (dx * dx + dy * dy) < (ssRadius * ssRadius); 
}


void main()
{
	vec3 defaultColor = dfShadingAmount * texture2D(sampler, uv).xyz;

	// step 1: get depth
	float depth = textureLod(momentsSampler, uv, 0.).x;
	
	// step 2: claculate r:
	vec4 rVec1 = projectionMatrix * vec4(0., worldSpaceRadius, -depth * farPlane, 1.);
	vec4 rVec2 = projectionMatrix * vec4(0., 0, -depth * farPlane, 1.);
	rVec1 /= rVec1.w;
	rVec2 /= rVec2.w;
	float r = abs(rVec1.y - rVec2.y);
	
	// step 3: calculate mipMapLevel:1-r)) + 3.;
	
	// step 4: get filtered Moments
	vec4 moments = textureLod(momentsSampler, uv, mmLevel);
	
	float mean = moments.x;
	float variance = sqrt(moments.y - pow(mean,2));
	
	
	
	//outputColor = vec4(defaultColor.x, defaultColor.y, isInCenterEpsilonArea(mmLevel*0.1), 1.);
    //outputColor = vec4(0, 0, mmLevel , 1.);
    outputColor = vec4(0.,0.,variance, 1.);
    
}
