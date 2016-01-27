#version 150

// this is a debug shader:
in vec2 uv;
uniform mat4 projectionMatrix;

uniform sampler2D sampler;
uniform sampler2D ovSampler;
uniform sampler2D depthBuffer;
//uniform float time;

out vec4 outputColor;

const int samples = 10; //TODO, pass through shader
const float PI = 3.1415926536;
const float eps = 10e-8;

// factors to (linear) merge default shading
// and volumetric obscurance 
const float voShadingAmount = 1.;
const float dfShadingAmount = 1.;

// misc:

void main()
{    
    // render just Depth:
    outputColor = vec4((1.0 - texture2D(depthBuffer, uv).x) * vec3(1.,1.,1.), 1.);
}
