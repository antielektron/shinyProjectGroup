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
	float mmLevel = log2(1./(1. - r)) + 3;
	// step 4: get filtered Moments
	vec4 moments = textureLod(momentsSampler, uv, mmLevel);
	
	float mean = moments.x;
	float variance = sqrt(moments.y - pow(mean,2));
	// step 5: where the magic happens
	
    vec4 b = mix(moments, vec4(0.5,0.5,0.5,0.5), 3e-5);
    vec3 z;
    z.x = depth-0.005;
    float L32D22= -b.x * b.y + b.z;
    float D22= -b.x * b.x + b.y;
    float SquaredDepthVariance=-b.y * b.y + b.w;
    float D33D22=dot(vec2(SquaredDepthVariance,-L32D22),
                     vec2(D22,                  L32D22));
    float InvD22=1.0/D22;
    float L32=L32D22*InvD22;
    vec3 c=vec3(1.0,z.x,z.x*z.x);
    c.y-=b.x;
    c.z-=b.y+L32*c.y;
    c.y*=InvD22;
    c.z*=D22/D33D22;
    c.y-=L32*c.z;
    c.x-=dot(c.yz,b.xy);
    float p=c.y/c.z;
    float q=c.x/c.z;
    float r2=sqrt((p*p*0.25)-q);
    z.y=-p*0.5-r2;
    z.z=-p*0.5+r2;
    vec4 Switch=
        (z.z<z.x)?vec4(z.y,z.x,1.0,1.0):(
        (z.y<z.x)?vec4(z.x,z.y,0.0,1.0):
        vec4(0.0,0.0,0.0,0.0));
    float Quotient=(Switch.x*z.z-b.x*(Switch.x+z.z)+b.y)
                  /((z.z-Switch.y)*(z.x-z.y));
     float result = 1-clamp(Switch.z+Switch.w*Quotient,0,1);
    //return in4Moments.w;

	
	
	//outputColor = vec4(defaultColor.x, defaultColor.y, isInCenterEpsilonArea(mmLevel*0.1), 1.);
    //outputColor = vec4(0, 0, mmLevel , 1.);
    outputColor = vec4(0.,0.,variance, 1.);
    
}
