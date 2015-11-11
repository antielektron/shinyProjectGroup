#version 130
//fragment phong shader, see also: https://en.wikipedia.org/wiki/Phong_reflection_model

in vec3 normal;             // aka N
out vec4 worldPosition;     //aka -v
uniform vec3 v_lightDirection;   //aka L
uniform vec3 v_lightColor;

uniform vec4 color;

out vec4 outputColor;

void main()
{
    //TODO: make the following fixed values variable (taken from openGL superbible):
    float k_s = 0.2; //amount of specular reflection;
    float k_d = 0.9; //amount of diffuse refelction;
    float alpha = 3; //material constant

    vec3 n = normalize(normal);
    vec3 l = normalize(-1.0 *v_lightDirection);
    vec3 c = v_lightColor;
    vec3 v = normalize(-worldPosition.xyz);

    vec3 r = reflect(l,n);

    vec3 diffuse = (max(dot(n,l), 0.0) * k_d) * c;
    vec3 specular = (pow(max(dot(r,v),0.0), alpha) * k_s) * c;

    vec4 normalColored = vec4(normalize(normal*normal),1.); // color;
    outputColor = 0.75 * vec4(diffuse + specular, 1.0) + 0.25 * normalColored;
}
