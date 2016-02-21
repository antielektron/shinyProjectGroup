#version 150

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D momentsSampler;
uniform sampler2D depthBuffer;
//uniform float time;

out vec4 outputColor;

// very stupid perspective scaling approximation function:
float get_kx(float z)
{
	// physically not correct
	vec4 e1 = vec4(1., 0., z, 1.);
	vec4 e2 = vec4(0., 0., z, 1.);
	e1 = projectionMatrix * e1;
	e2 = projectionMatrix * e2;
	
	return 0.001 * (e2.x / e2.w - e1.x / e1.w);

}

float get_ky(float z)
{
	// physically not correct
	vec4 e1 = vec4(0., 1., z, 1.);
	vec4 e2 = vec4(0., 0., z, 1.);
	e1 = projectionMatrix * e1;
	e2 = projectionMatrix * e2;
	

	return 0.001 * (e2.y / e2.w - e1.y / e1.w);	
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

float get_depth(vec2 uv)
{
	// workaround till i know why there are 
	// such strange values in the z-buffer
	return texture2D(sampler, uv).a;
}

// debug stuff:

bool isInCenterEpsilonArea(vec2 centerPoint)
{
	float zCenter = (1. - textureLod(momentsSampler, vec2(0.5,0.5), 0).x) * .1;
	float kx = get_kx(zCenter);
	float ky = get_ky(zCenter);
	
	// aspect ratio:
	float ratio = ky / kx;
	
	
	float dx = (uv.x - centerPoint.x) * ratio;
	float dy = (uv.y - centerPoint.y);
	
	return (dx * dx + dy * dy) < (zCenter * zCenter); 
}



void main()
{

	// temporary solution for demo:
	// get depth
	float depth = textureLod(momentsSampler, uv, 0).x;
	
	// detemine screenspace depth (not physically correct so far)
	float r = (1. - depth) * 0.01;
	
	// get mipmap level for this area
	float mmLevel = log2(1. / r);
	
	vec4 moments = textureLod(momentsSampler, uv,mmLevel);
	float mean = moments.x;
	float variance = sqrt(moments.y - pow(mean,2));


	float a = -1.0 / (2. * variance);
	float b = -a * (mean + variance);
	
	// nonsense... but it could make almost valid results
	float z0 = depth - r;
	float z1 = depth + r;
	
	// aaaand i have no idea what i'm doing now:
	float voFactor =  a * (pow(z1,2) - pow(z0,2)) / 2.0 + b * (z1 - z0);
		
    vec3 mixedColor = vec3(10* mean, 10 * variance,10 * variance);
    //vec3 mixedColor = vec3(10* (mean - d), 0.,0.);
    //vec3 mixedColor = vec3(1.,1.,1.) - clamp(10 * voFactor * vec3(1.,1.,1.), 0, 1);
    
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
	
    
    if (isCursor())
	{
		mixedColor = vec3(1.,1.,1.) - mixedColor;
	}    
	
	outputColor = vec4( mixedColor, 1.);
}
