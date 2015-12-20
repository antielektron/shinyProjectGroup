#version 130

// gl_FragCoord.z sucks
in vec4 lightViewPosition;

out float fragDepth; // aka depth

void main()
{
    // z buffer from -1 to 1!!!
    // using original depth gives worse results..
    fragDepth = lightViewPosition.z*0.5 + 0.5; // gl_FragCoord.z;
}
