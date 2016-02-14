#version 150
#extension GL_EXT_texture_array : enable

in vec2 uv;

uniform int layer;

uniform sampler2DArray sampler;

out vec4 outputColor;


void sampleOptimized4MomentsShadowMap(out vec4 out4Moments, vec4 shadowMapValue)
{
    shadowMapValue.x -= 0.035955884801f;

    mat4 transformation2 = transpose(mat4( 0.2227744146,   0.0771972861,   0.7926986636,   0.0319417555,
                                            0.1549679261,   0.1394629426,   0.7963415838,   -0.172282317,
                                            0.1451988946,   0.2120202157,   0.7258694464,   -0.2758014811,
                                            0.163127443,    0.2591432266,   0.6539092497,   -0.3376131734 ));

    out4Moments = transformation2 * shadowMapValue;
}

void main()
{
    if (true)
    {
        outputColor = texture2DArray(sampler, vec3(uv, layer));
    }
    else
    {
        vec4 texValue = texture2DArray(sampler, vec3(uv, layer));
        sampleOptimized4MomentsShadowMap(outputColor, texValue);
    }
}
