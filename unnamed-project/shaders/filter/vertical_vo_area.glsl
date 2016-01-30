#version 430

layout (binding=0, rgba8) readonly uniform image2D sourceImage;
layout (binding=1, rgba8) writeonly uniform image2D filteredImage;

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
    0.05, 0.05, 0.05, 0.05, 0.05,
    0.05, 0.05, 0.05, 0.05, 0.05,
    0.05, 0.05, 0.05, 0.05, 0.05,
    0.05, 0.05, 0.05, 0.05, 0.05, 0.05
};

layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    vec4 result = vec4(0, 0, 0, 0);
	
	int halfKernel = (kernelSize - 1)/2;

    for (int i = 0; i < kernelSize; i++)
    {
        vec4 value = imageLoad(sourceImage, pos + ivec2(0, i - halfKernel));
        result += value * weights[i];
    }

    imageStore(filteredImage, pos, result);
}
	