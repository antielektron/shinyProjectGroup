#version 130

// gl_FragCoord.z sucks
in vec4 lightViewPosition;

out float fragDepth; // aka depth

void main()
{
    // lightViewPosition from -1 to 1!
    fragDepth = lightViewPosition.z*0.5 + 0.5; // lightViewPosition.z*0.5 + 0.5 = gl_FragCoord.z;
}
