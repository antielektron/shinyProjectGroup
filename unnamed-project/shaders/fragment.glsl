#version 130
//fragment phong shader, see also: https://en.wikipedia.org/wiki/Phong_reflection_model

in vec3 normal;             // aka N
in vec4 worldPosition;     //aka -v
uniform vec3 v_lightDirection;   //aka L
uniform vec3 v_lightColor;

uniform float k_s;          //specular amount
uniform float k_d;          //diffuse amount
uniform float mat_alpha;        //alpha?

uniform vec4 color;

out vec4 outputColor;

void main()
{
    vec3 n = normalize(normal);
    vec3 l = normalize(-1.0 *v_lightDirection);
    vec3 c = v_lightColor;
    vec3 v = normalize(-worldPosition.xyz);

    //reflection
    vec3 r = normalize((2 * dot(l,n))* n - l);

    vec3 diffuse = (max(dot(n,l), 0.0) * k_d) * c;
    vec3 specular = (pow(max(dot(r,v),0.0), mat_alpha) * k_s) * c;
    vec4 normalColored = vec4(normalize(normal*normal),1.); // color;
    outputColor = 1.0 * vec4(diffuse + specular, 1.0) + 0.0 * normalColored;
}
