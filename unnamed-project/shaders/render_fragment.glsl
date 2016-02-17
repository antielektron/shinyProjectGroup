#version 150
#extension GL_EXT_texture_array : enable

in vec3 normal;
in vec3 worldPosition;
in float eyeZ;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;

// uniform mat4 cascadeViewMatrix[4];
layout (binding = 0) buffer cascadeViewBuffer
{
    mat4 cascadeViewMatrix[4];
};

// uniform float cascadeFar[4]; // where do the cascades end?
layout (binding = 1) buffer cascadeFarBuffer
{
    vec4 cascadeFar; // NOTE: float[4] will be ill aligned..
};

// gl_TextureMatrix
uniform sampler2DArray shadowMapSampler;

const float roughness = 0.1;
const float fresnelFactor = 1.;

out vec4 fragColor;
out vec2 moments;

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

int getCascade()
{
    // float z = -worldPosition.z;
    float z = gl_FragCoord.z;

    if (z < cascadeFar[0])
        return 0;
    else if (z < cascadeFar[1])
        return 1;
    else if (z < cascadeFar[2])
        return 2;
    else
        return 3;
}

void sampleOptimized4MomentsShadowMap(out vec4 out4Moments, vec4 shadowMapValue)
{
    shadowMapValue.x -= 0.035955884801f;
    out4Moments = transpose(mat4( 0.2227744146,   0.0771972861,   0.7926986636,   0.0319417555,
                    0.1549679261,   0.1394629426,   0.7963415838,   -0.172282317,
                    0.1451988946,   0.2120202157,   0.7258694464,   -0.2758014811,
                    0.163127443,    0.2591432266,   0.6539092497,   -0.3376131734 ))
                    * shadowMapValue;

}

float computeMSMShadwowIntensity(vec4 in4Moments, float depth, float depthBias, float momentBias)
{
    vec4 b = mix(in4Moments, vec4(0.5,0.5,0.5,0.5), momentBias);
    vec3 z;
    z.x = depth-depthBias;
    float L32D22= -b.x * b.y + b.z;
    float D22= -b.x * b.x + b.y;
    float SquaredDepthVariance=-b.y * b.y + b.w;
    float D33D22=dot(vec2(SquaredDepthVariance,-L32D22),
                     vec2(D22,                  L32D22));
    float InvD22=1.0/D22;
    float L32=L32D22*InvD22;
    vec3 c=vec3(1.0,z.x,z.x*z.x);
    c.y-=b.x;
    c.z-=b.y+L32*c.y;
    c.y*=InvD22;
    c.z*=D22/D33D22;
    c.y-=L32*c.z;
    c.x-=dot(c.yz,b.xy);
    float p=c.y/c.z;
    float q=c.x/c.z;
    float r=sqrt((p*p*0.25)-q);
    z.y=-p*0.5-r;
    z.z=-p*0.5+r;
    vec4 Switch=
        (z.z<z.x)?vec4(z.y,z.x,1.0,1.0):(
        (z.y<z.x)?vec4(z.x,z.y,0.0,1.0):
        vec4(0.0,0.0,0.0,0.0));
    float Quotient=(Switch.x*z.z-b.x*(Switch.x+z.z)+b.y)
                  /((z.z-Switch.y)*(z.x-z.y));
    return 1-clamp(Switch.z+Switch.w*Quotient,0,1);
    //return in4Moments.w;
}

float simpleShadowTerm()
{
    // Find the correct cascade..
    int index = getCascade();

    vec4 lightViewPosition = cascadeViewMatrix[index] * vec4(worldPosition, 1.);
    vec2 uv = vec2(lightViewPosition.xy * 0.5 + 0.5);
    vec4 shadowMapValue = texture2DArray(shadowMapSampler, vec3(uv, index));
    float depth = lightViewPosition.z*0.5 + 0.5;
    vec4 shadowMapMoments;
    sampleOptimized4MomentsShadowMap(shadowMapMoments, shadowMapValue);
    return computeMSMShadwowIntensity(shadowMapMoments, depth, 0.005, 3e-5);
    //Depth Bias Surface Acne verhindern, in Demo 0
    // Add some epsilon
    //if (depth - shadowMapDepth <= 0.005)
    //    return 1.;
    //else
        //return 0.1;
}

void main()
{
    vec3 n = normalize(normal);
    vec3 l = normalize(lightDirection);
    vec3 v = normalize(-worldPosition); // eye is in the origin.
    float diffuseTerm = cdot(n, l);

    float specularTerm = cooktorranceTerm(v, n, l);

    float shadowTerm = simpleShadowTerm();

    vec3 shadedColor = clamp(specularTerm * specularColor + diffuseTerm * diffuseColor, 0., 1.);

    float scaledZ = gl_FragCoord.z*gl_FragCoord.w;
    fragColor = vec4(clamp(shadowTerm * (shadedColor) + ambientColor, 0., 1.), scaledZ);

        /*
    int index = getCascade();
    if (index == 0)
        fragColor.xyz *= vec3(1., 0., 0.);
    else if (index == 1)
        fragColor.xyz *= vec3(0., 1., 0.);
    else if (index == 2)
        fragColor.xyz *= vec3(0., 0., 1.);
    else if (index == 3)
        fragColor.xyz *= vec3(1., 1., 0.);
        */

    moments = vec2(scaledZ, pow(scaledZ,2));
}
