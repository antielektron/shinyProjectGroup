#version 150

in vec2 uv;
uniform mat4 projectionMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D sampler;
uniform sampler2D momentsSampler;
uniform sampler2D depthBuffer;
uniform vec4 lightDirection;
uniform float ratio;
//uniform float time;

out vec4 outputColor;

const float PI = 3.1415926536;
const float eps = 10e-8;
const float verticalViewAngle = PI/4;

float get_angle(vec3 a, vec3 b)
{
	// cross product's squared length:
	float c = pow(a.y * b.z - a.z * b.y,2)
	        + pow(a.z * b.x - a.x * b.z,2)
            + pow(a.x * b.y - a.y * b.x,2);
    c = sqrt(c);
    float abs_a = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    float abs_b = sqrt(b.x * b.x + b.y * b.y + b.z * b.z);

    float sin_theta = c/(abs_a * abs_b + 10e-6);

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
	float theta = verticalViewAngle * ratio * (uv.x - 0.5) * 2.;
	float rho = verticalViewAngle * (uv.y - 0.5) * 2.;
	
	vec3 uvAngle = vec3(sin(theta),sin(rho),-1.); 
	
	// angle should be < 1 since we use a viewAngle = PI/2;
	float angle = get_angle(uvAngle, lightDirection.xyz);
	float sqrdAngle = angle * angle;
	
	float g = 1 - sqrdAngle * 0.1;
	float r = 1 - angle;
	float b = 1 - sqrdAngle * 0.1;
	
	
	return vec3(r,g,b);
}

void main()
{
	vec3 defaultColor = texture2D(sampler, uv).xyz;

	if (texture2D(depthBuffer, uv).x == 1)
	{
		defaultColor = get_sky();
	}
	
	outputColor = vec4(defaultColor, 1.);
}
