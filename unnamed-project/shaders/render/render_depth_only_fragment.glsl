#version 150

out vec2 moments;
out vec2 moments2;
uniform mat4 inverseProjectionMatrix;
const float sceneDepth = 200;
const float nearPlaneDepth = 3;
const float correction = 0.2;

float get_world_depth(float z)
{
	vec4 result = inverseProjectionMatrix * vec4(0, 0, z, 1);
	return - result.z/result.w;
}

float get_linearized_depth(float world_z)
{
	return (world_z) / (sceneDepth);
}

void main()
{
    float z = get_linearized_depth(get_world_depth(gl_FragCoord.z))*(1-correction) + correction;
    //float z =  gl_FragCoord.z * 100;
    vec4 tempmoments = vec4(z, pow(z,2), pow(z,3), pow(z,4));
    
    vec4 fragMoments = transpose(mat4(	-2.07224649,	32.23703778,	-68.571074599,	39.3703274134,
                                        13.7948857237,	-59.4683975703, 82.0359750338,	-35.364903257,
                                        0.105877704,	-1.9077466311,	9.3496555107,	-6.6543490743,
                                        9.7924062118,	-33.7652110555,	47.9456096605,	-23.9728048165))
						    * tempmoments;
	fragMoments.x += 0.035955884801;
	moments = tempmoments.xy;
	moments2 = tempmoments.zw;   
}
