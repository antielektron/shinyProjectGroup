#version 450

layout (binding=0, rgba16) readonly uniform image1DArray inputTex;

layout (binding = 1) writeonly buffer cascadeFarBuffer
{
    float cascadeFar[4];
};


layout (local_size_x = 4) in;

void main()
{

}
