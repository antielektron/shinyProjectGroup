#version 430

layout (binding=0, rg16) readonly uniform image2D sourceImage;
layout (binding=1, rg16) writeonly uniform image2D filteredImage;

uniform mat4 projectionMatrix;

// very stupid perspective scaling approximation function:
float get_kx(float z)
{
	// physically not correct
	vec4 e1 = vec4(1., 0., z, 1.);
	vec4 e2 = vec4(0., 0., z, 1.);
	e1 = projectionMatrix * e1;
	e2 = projectionMatrix * e2;
	
	return 0.01;
	//return 0.1 * (e2.x / e2.w - e1.x / e1.w);

}

float get_ky(float z)
{
	// physically not correct
	vec4 e1 = vec4(0., 1., z, 1.);
	vec4 e2 = vec4(0., 0., z, 1.);
	e1 = projectionMatrix * e1;
	e2 = projectionMatrix * e2;
	
	return 0.01;
	//return 0.1 * (e2.y / e2.w - e1.y / e1.w);	
}

// first test, make it constant. will be dependend from k_x and k_y in future
const int kernelSize = 21;

// for simplicity and testing, my "gauss"-filter looks like:
const float weights[kernelSize] = {
    0.004481, 0.0080891, 0.013722, 0.021874, 0.032768,
    0.046128, 0.061021, 0.075856, 0.088613, 0.097274,
    0.100346, 0.097274, 0.088613, 0.075856, 0.061021,
    0.046128, 0.032768, 0.021874, 0.013722, 0.008089,
    0.004481
};

layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    vec4 result = vec4(0, 0, 0, 0);
	
	int halfKernel = (kernelSize - 1)/2;

    for (int i = 0; i < kernelSize; i++)
    {
        vec4 value = imageLoad(sourceImage, pos + ivec2(i - halfKernel, 0));
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}
