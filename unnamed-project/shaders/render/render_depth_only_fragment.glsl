#version 150

out vec4 moments;

void main()
{
    float z = 1 - gl_FragCoord.z * gl_FragCoord.w;
    moments = vec4(z, pow(z,2), pow(z,3), pow(z,4));
}
