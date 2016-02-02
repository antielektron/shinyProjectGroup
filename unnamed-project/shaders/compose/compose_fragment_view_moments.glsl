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
	vec4 moments = texture2D(momentsSampler, uv);
	float mean = moments.x;
	float variance = sqrt(moments.y - pow(mean,2));
	
    vec3 mixedColor = vec3(mean, variance, variance);
    if (isCursor())
	{
		mixedColor = vec3(1.,1.,1.) - mixedColor;
	}    
	
	outputColor = vec4( mixedColor, 1.);
}
