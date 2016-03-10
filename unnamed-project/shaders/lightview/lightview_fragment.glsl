#version 430
#extension GL_EXT_texture_array : enable

in vec3 normal;

uniform vec3 diffuseColor;
uniform vec3 ambientColor;

out vec4 fragColor;

// Clamp result to [0, 1]
float cdot(vec3 a, vec3 b)
{
    return clamp(dot(a, b), 0.0, 1.0);
}

void main()
{
    vec3 n = normalize(normal);
    float diffuseTerm = cdot(n, vec3(0, 0, 1));

    fragColor = vec4(clamp(diffuseTerm * diffuseColor + ambientColor, 0., 1.), 1);
}
