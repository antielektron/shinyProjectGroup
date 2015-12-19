#version 130

// gl_FragCoord.z sucks
in vec4 lightViewPosition;

out float fragDepth; // aka depth

void main()
{
    // using original depth gives worse results..
    fragDepth = -lightViewPosition.z; // gl_FragCoord.z;
}
