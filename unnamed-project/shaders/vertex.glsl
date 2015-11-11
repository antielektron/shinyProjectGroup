#version 130

in vec3 v_position;
in vec3 v_normal;

uniform mat4 modelView;
uniform mat4 projection;

out vec3 normal;
out vec4 worldPosition;

void main()
{
    //lightDirection = v_lightDirection;// vec3(0.0,0.0,-1.0);
    //lightColor = v_lightColor; //vec3(0.0,1.0,1.0);
    worldPosition = modelView * vec4(v_position, 1.);
    gl_Position = projection * worldPosition;
    normal = (modelView * vec4(v_normal, 0.)).xyz;
}
