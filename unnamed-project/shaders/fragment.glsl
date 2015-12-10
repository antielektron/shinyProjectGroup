#version 130

in vec3 normal;
in vec3 world;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;

const float roughness = 0.4;
const float refractionIndex = 0.4;

out vec4 fragColor;

const float pi = 3.14159265359;

// Clamp result to [0, 1]
float cdot(vec3 a, vec3 b)
{
    return clamp(dot(a, b), 0.0, 1.0);
}


float beckmannDistribution(float dotNH)
{
    if (dotNH == 0.)
        return 0.;
    float sigma2 = roughness * roughness;
    float m = 2.;
    float alpha = acos(dotNH);
    float temp = tan(alpha)/m;
    float nom = exp(-temp*temp);
    float denom = pi*m*m*pow(dotNH, 4.);
    return nom/denom;
}

float schlickApprox(float dotVH, float n1, float n2)
{
    float r = (n1-n2)*(n1-n2)/(n1+n2)/(n1+n2);
    return r + (1.-r)*pow(1.-dotVH, 5.);
}

float geometricAttenuation(float dotNH, float dotVN, float dotVH, float dotNL)
{
    if (dotVH == 0.)
        return 0.;
    float g1 = 2.*dotNH*dotVN/dotVH;
    float g2 = 2.*dotNH*dotNL/dotVH;
    return min(1., min(g1, g2));
}

float cooktorranceTerm(vec3 v, vec3 n, vec3 l)
{
    // Im eye space ist die Richtung zum Betrachter schlicht die Z-Achse
    // TODO are we in "eye" or "camera"
    // vec3 v = vec3(0.0, 0.0, 1.0);
    vec3 h = normalize(l + v);

    // Precompute scalar products
    float dotVN = cdot(v, n);
    float dotNL = cdot(n, l);
    float dotNH = cdot(n, h);
    float dotVH = cdot(v, h);

    if (dotNL == 0. || dotVN == 0.)
        return 0.;

    float D = beckmannDistribution(dotNH);
    float F = schlickApprox(dotVH, 1.0, refractionIndex);
    float G = geometricAttenuation(dotNH, dotVN, dotVH, dotNL);

    return D * F * G / (4.0 * dotVN * dotNL);
}

void main()
{
    vec3 n = normalize(normal);
    vec3 l = normalize(-lightDirection);
    vec3 v = normalize(-world); // eye is in the origin.
    float diffuseTerm = cdot(n, l);

    float specularTerm = cooktorranceTerm(v, n, l);

    fragColor = vec4(clamp(specularTerm * specularColor + diffuseTerm * diffuseColor + ambientColor, 0., 1.), 1.);
    // fragColor = vec4(l, 1.);
}
