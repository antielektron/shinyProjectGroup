#version 430

// layout (binding=0, r16) readonly uniform image2DMS depthImage;
layout (binding=0) uniform sampler2DMSArray depthSampler;
layout (binding=1, rgba16) writeonly uniform image2DArray momentImage;

layout (local_size_x = 8, local_size_y = 8) in;

void main()
{
    ivec3 pos = ivec3(gl_GlobalInvocationID.xyz);

    vec4 moments = vec4(0, 0, 0, 0);

    /*
    vec2 size = vec2(imageSize(momentImage));
    vec3 normalPos = vec3((vec2(pos.xy) + 0.5) / size, pos.z);
    float depth = texture2DArray(depthSampler, normalPos).x;
    float square = depth * depth;
    moments = vec4(depth, square, square*depth, square*square);
    */
    for (int i = 0; i < 4; i++)
    {
        float depth = texelFetch(depthSampler, pos, i).x;
        float square = depth * depth;
        moments += vec4(depth, square, square*depth, square*square);
    }
    moments /= 4.;

	vec4 fragMoments = transpose(mat4(	-2.07224649,	32.23703778,	-68.571074599,	39.3703274134,
                                        13.7948857237,	-59.4683975703, 82.0359750338,	-35.364903257,
                                        0.105877704,	-1.9077466311,	9.3496555107,	-6.6543490743,
                                        9.7924062118,	-33.7652110555,	47.9456096605,	-23.9728048165))
						    * moments;
	fragMoments.x += 0.035955884801;

    imageStore(momentImage, pos, fragMoments);
}
