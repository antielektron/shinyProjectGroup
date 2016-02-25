#version 430

layout (binding = 0, rgba16) readonly uniform image1DArray inputTex;

layout (binding = 1) writeonly buffer cascadeViewBuffer
{
    mat4 cascadeViewMatrix[4];
};

uniform mat4 inverseTempProjectionMatrix;
uniform mat4 lightViewMatrix;

layout (local_size_x = 4) in;

void main()
{
    // TODO why nvidia only likes it this way
    vec3 minCorners[4];
    vec3 maxCorners[4];

    // 6 layer
    // minx miny minz maxx maxy maxz

    for (int index = 0; index < 3; index++)
    {
        vec4 minValue = imageLoad(inputTex, ivec2(0, index));
        minValue = minValue * 2 - 1;

        minCorners[0][index] = minValue[0];
        minCorners[1][index] = minValue[1];
        minCorners[2][index] = minValue[2];
        minCorners[3][index] = minValue[3];


        vec4 maxValue = imageLoad(inputTex, ivec2(0, index + 3));
        maxValue = maxValue * -2 + 1;

        maxCorners[0][index] = maxValue[0];
        maxCorners[1][index] = maxValue[1];
        maxCorners[2][index] = maxValue[2];
        maxCorners[3][index] = maxValue[3];

    }

    int index = int(gl_GlobalInvocationID.x);

    // only very few threads ;) ie 4
    // barrier();

    // construct the projection
    vec4 minCorner = inverseTempProjectionMatrix * vec4(minCorners[index], 1);
    vec4 maxCorner = inverseTempProjectionMatrix * vec4(maxCorners[index], 1);
    vec3 ext = maxCorner.xyz - minCorner.xyz;
    vec3 sum = minCorner.xyz + maxCorner.xyz;

    vec4 diag = vec4(2/ext, 0);
    vec4 trans = vec4(-sum/ext, 1);

    mat4 projection = mat4(
        diag.xwww,
        diag.wyww,
        diag.wwzw,
        trans);

    cascadeViewMatrix[index] = projection * lightViewMatrix;
}
