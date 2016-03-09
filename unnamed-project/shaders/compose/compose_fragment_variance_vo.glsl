#version 430

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D momentsSampler;
uniform sampler2D depthBuffer;
uniform float ratio;
uniform vec4 lightDirection;
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
const float worldSpaceRadius = 1;
const float sceneDepth = 1000;
const float verticalViewAngle = PI/4;


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

float get_world_depth(float z)
{
	return z * sceneDepth;
}

bool isInCenterEpsilonArea(float ssRadius)
{
	
	
	float dx = (uv.x - 0.5);
	float dy = (uv.y - 0.5);
	
	return (dx * dx + dy * dy) < (ssRadius * ssRadius); 
}

void sampleOptimized4MomentsShadowMap(out vec4 out4Moments, vec4 shadowMapValue)
{
    shadowMapValue.x -= 0.035955884801f;
    out4Moments = transpose(mat4( 0.2227744146,   0.0771972861,   0.7926986636,   0.0319417555,
                    0.1549679261,   0.1394629426,   0.7963415838,   -0.172282317,
                    0.1451988946,   0.2120202157,   0.7258694464,   -0.2758014811,
                    0.163127443,    0.2591432266,   0.6539092497,   -0.3376131734 ))
                    * shadowMapValue;

}


float f(vec4 moments, float z_a, float z_b)
{
	float mean = moments.x;
	float sigma = sqrt(moments.y - pow(mean,2));	
	
	float a = (-1.)/(2* sigma);
	float b = -a*(mean + sigma);
	
	return a*(z_b * z_b - z_a * z_a)/2 + b * (z_b - z_a);
}

float get_angle(vec3 a, vec3 b)
{
	// cross product's squared length:
	float c = pow(a.y * b.z - a.z * b.y,2)
	        + pow(a.z * b.x - a.x * b.z,2)
            + pow(a.x * b.y - a.y * b.x,2);
    c = sqrt(c);
    float abs_a = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    float abs_b = sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
    
    float sin_theta = c/(abs_a * abs_b);
    
    // check Orientation:
   	bool sameOrientation = false;
   	if (a.x * b.x + a.y * b.y + a.z * b.z > 0)
    {
    		sameOrientation = true;
    }
    
    return sameOrientation ? asin(sin_theta) : PI - asin(sin_theta);
}

vec3 get_sky()
{
	float theta = verticalViewAngle * (uv.x - 0.5) * 2.;
	float rho = verticalViewAngle * ratio * (uv.y - 0.5) * 2.;
	
	vec3 uvAngle = vec3(sin(theta),sin(rho),-1.); 
	
	// angle should be < 1 since we use a viewAngle = PI/2;
	float angle = get_angle(uvAngle, lightDirection.xyz);
	float sqrdAngle = angle * angle;
	
	float g = 0.7 - sqrdAngle * 0.1;
	float r = 1 - angle;
	float b = sqrdAngle * 0.2;
	
	return vec3(r,g,b);
}

void main()
{	
	vec3 defaultColor = dfShadingAmount * texture2D(sampler, uv).xyz;

	// step 1: get depth
	float depth = textureLod(momentsSampler, uv,0).x;
	
	float world_depth = get_world_depth(depth);
	
	// step 2: claculate r:
	vec4 rVec1 = projectionMatrix * vec4(0., 2 * worldSpaceRadius, world_depth, 1.);
	vec4 rVec2 = projectionMatrix * vec4(0., 0, world_depth, 1.);
	
	float r = abs(rVec1.y/rVec1.w - rVec2.y/rVec2.w);
	
	// step 3: calculate mipMapLevel:1-r)) + 3.;
	float mmLevel = 1 - log2(1./(1. - r));
	// step 4: get filtered Moments
	mmLevel = mmLevel * 0.9 + 2;
	vec4 moments = textureLod(momentsSampler, uv, 3.2);
	vec4 outMoments;
	sampleOptimized4MomentsShadowMap(outMoments, moments);
	

	// step 5: where the magic happens
	float z_a = depth + 0.1;
	float z_b = depth - 0.1;
	
	float sum = f(outMoments,z_a, z_b); 
	
	if (depth < 1-10e-4)
	{
		defaultColor =  sum * vec3(1.,1.,1.);//defaultColor;
	}
	else
	{
		defaultColor = get_sky();
	}
	
	//outputColor = vec4(0, defaultColor.y, 0,1);
	//outputColor = vec4(momentMagic, 0, 0,1);
    outputColor = vec4(defaultColor.x,0.0,isInCenterEpsilonArea(sum * 0.5),1);
    outputColor = vec4(defaultColor, 1.0);
    //outputColor = vec4(0.,0.,1-result * 0.5, 1.);
    
}
