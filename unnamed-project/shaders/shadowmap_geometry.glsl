#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

// uniform mat4 cascadeViewMatrix[4];
layout (binding = 0) buffer cascadeViewBuffer
{
    mat4 cascadeViewMatrix[4];
};

void main()
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            gl_Position = cascadeViewMatrix[j] * gl_in[i].gl_Position;
            gl_Layer = j;
            EmitVertex();
        }
        EndPrimitive();
    }
}
