#version 130

in vec3 v_position;
in vec3 v_normal; // unused..

uniform mat4 lightViewMatrix;

out vec4 lightViewPosition;

void main()
{
    gl_Position = lightViewPosition = lightViewMatrix * vec4(v_position, 1.);
    // Don't clip objects that are "before" the frustum
    if (gl_Position.z < -1.)
        gl_Position.z = -1.;
}
