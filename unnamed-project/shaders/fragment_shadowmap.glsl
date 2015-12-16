#version 130

out float fragDepth; // aka depth

void main()
{
    fragDepth = gl_FragCoord.z;
}
