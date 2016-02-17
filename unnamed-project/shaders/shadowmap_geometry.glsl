#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

// uniform mat4 cascadeViewMatrix[4];
layout (binding = 0) buffer cascadeViewBuffer
{
    mat4 cascadeViewMatrix[4];
};

out vec4 lightViewPosition;

// NOTE: Geometry shader is not needed to draw color and normal into two textures!! (maybe required for shadow maps when using texture arrays)

// TODO disable near clipping in opengl "hardware"
// opengl clampt dann die z werte!

void main()
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            lightViewPosition = cascadeViewMatrix[j] * gl_in[i].gl_Position;
            gl_Position = lightViewPosition;
            // Don't clip objects that are "before" the frustum
            // NOTE: extend frustum towards the light!!!
            if (gl_Position.z < -1.)
                gl_Position.z = -1.;

            gl_Layer = j;
            EmitVertex();
        }
        EndPrimitive();
    }
}
