#version 150

out vec2 moments;

void main()
{
    float z = gl_FragCoord.z * gl_FragCoord.w;
    moments = vec2(z, pow(z,2));
}
