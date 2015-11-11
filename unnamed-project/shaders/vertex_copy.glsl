#version 130

in vec2 v_position;

// TODO matrix that stretches to fullscreen!

out vec2 uv;

void main()
{
    // static transformation to viewport!
    gl_Position = vec4(v_position*2. - 1., 0., 1.);
    uv = v_position;
}
