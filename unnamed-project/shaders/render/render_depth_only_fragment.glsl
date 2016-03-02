#version 150

out vec4 moments;

void main()
{
    float z = (gl_FragCoord.z * gl_FragCoord.w);
    //float z =  gl_FragCoord.z * 100;
    vec4 tempmoments = vec4(z, pow(z,2), pow(z,3), pow(z,4));
    
    vec4 fragMoments = transpose(mat4(	-2.07224649,	32.23703778,	-68.571074599,	39.3703274134,
                                        13.7948857237,	-59.4683975703, 82.0359750338,	-35.364903257,
                                        0.105877704,	-1.9077466311,	9.3496555107,	-6.6543490743,
                                        9.7924062118,	-33.7652110555,	47.9456096605,	-23.9728048165))
						    * tempmoments;
	fragMoments.x += 0.035955884801;
	moments = fragMoments;    
}
