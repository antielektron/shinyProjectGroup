#version 150

in vec3 normal;
in vec3 worldPosition;
in vec4 lightViewPosition;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;

uniform sampler2D shadowMapSampler;

const float roughness = 0.1;
const float fresnelFactor = 1.;

out vec4 fragColor;
out vec4 fragNormalDepth;

const float PI = 3.14159265359;

// Clamp result to [0, 1]
float cdot(vec3 a, vec3 b)
{
    return clamp(dot(a, b), 0.0, 1.0);
}

float chi(float v)
{
    return v > 0 ? 1 : 0;
}

float DTerm(float dotNH, float alpha)
{
/*
    float alpha2 = alpha * alpha;
    float dotNH2 = dotNH * dotNH;

    float denom = dotNH2 * alpha2 + 1. - dotNH2;

    return chi(dotNH) * alpha2 / (PI * denom * denom);
*/
    float alpha2 = alpha * alpha;
    float dotNH2 = dotNH * dotNH;

    float nom = exp((dotNH2 - 1.) / (alpha2 * dotNH2));

    return nom / (PI * dotNH2 * dotNH2 * alpha2);
}

float FTerm(float dotVH, float F0)
{
    return F0 + (1-F0) * pow( 1 - dotVH, 5);
}

float GTerm(float dotNH, float dotVN, float dotNL, float dotVH, float dotLH)
{
    float g1 = dotVH == 0. ? 1. : 2.*dotNH*dotVN/dotVH;
    float g2 = dotLH == 0. ? 1. : 2.*dotNH*dotNL/dotLH;
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
    float dotLH = cdot(l, h);

    if (dotNL <= 1e-6 || dotVN < 1e-6)
        return 0.;

    float D = DTerm(dotNH, roughness);
    float F = FTerm(dotVH, fresnelFactor); // F0
    float G = GTerm(dotNH, dotVN, dotNL, dotVH, dotLH);

    return D * F * G / (4.0 * dotVN * dotNL);
}

float simpleShadowTerm(vec2 lv, float d)
{
    vec2 uv = vec2(lv * 0.5 + 0.5);
    float shadowMapDepth = texture2D(shadowMapSampler, uv).x;
    float depth = d*0.5 + 0.5;

    // Add some epsilon
    if (depth - shadowMapDepth <= 0.01)
        return 1.;
    else
        return 0.;
}

void main()
{
    vec3 n = normalize(normal);
    vec3 l = normalize(lightDirection);
    vec3 v = normalize(-worldPosition); // eye is in the origin.
    float diffuseTerm = cdot(n, l);

    float specularTerm = cooktorranceTerm(v, n, l);

    vec2 lv = lightViewPosition.xy;
    float d = lightViewPosition.z;
    float shadowTerm = simpleShadowTerm(lv, d);

    fragColor = vec4(clamp(shadowTerm * (specularTerm * specularColor + diffuseTerm * diffuseColor) + ambientColor, 0., 1.), 1.);

    fragNormalDepth = vec4(normal*0.5 + 0.5, gl_FragCoord.z*gl_FragCoord.w);
}
