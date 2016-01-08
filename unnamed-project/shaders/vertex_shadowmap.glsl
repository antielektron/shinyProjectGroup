#version 150

in vec3 v_position;
in vec3 v_normal; // unused..

uniform mat4 worldMatrix;

void main()
{
    // Copy
    gl_Position = worldMatrix * vec4(v_position, 1.);

    // outData.lightViewPosition = lightViewMatrix * vec4(v_position, 1.);
    // Don't clip objects that are "before" the frustum
    // if (gl_Position.z < -1.)
    //     gl_Position.z = -1.;
}
