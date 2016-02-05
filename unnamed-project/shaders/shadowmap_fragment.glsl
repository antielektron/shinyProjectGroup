#version 130

// gl_FragCoord.z sucks
in vec4 lightViewPosition;

out vec4 fragMoments; // aka depth


void main()
{
    // lightViewPosition from -1 to 1!

    float depth = lightViewPosition.z*0.5 + 0.5;

    if (depth < 0.)
        depth = 0.;

    float square = depth*depth;
	vec4 moments = vec4(depth,square,square*depth,square*square);
	fragMoments = transpose(mat4(	-2.07224649,	32.23703778,	-68.571074599,	39.3703274134,
						13.7948857237,	-59.4683975703, 82.0359750338,	-35.364903257,
						0.105877704,	-1.9077466311,	9.3496555107,	-6.6543490743,
						9.7924062118,	-33.7652110555,	47.9456096605,	-23.9728048165))
						* moments;
	fragMoments.x += 0.035955884801;
    // fragDepth = lightViewPosition.z*0.5 + 0.5; // lightViewPosition.z*0.5 + 0.5 = gl_FragCoord.z;
    // alpha blending ausschalten
}
