void Get4MomentsOptimized(out float4 Out4MomentsOptimized,float FragmentDepth)
{
    float Square=FragmentDepth*FragmentDepth;
    float4 Moments=float4(FragmentDepth,Square,Square*FragmentDepth,Square*Square);
    Out4MomentsOptimized=mul(Moments,float4x4(
                     -2.07224649f,   13.7948857237f,  0.105877704f,   9.7924062118f,
                     32.23703778f,  -59.4683975703f, -1.9077466311f,-33.7652110555f,
                    -68.571074599f,  82.0359750338f,  9.3496555107f, 47.9456096605f,
                     39.3703274134f,-35.364903257f,  -6.6543490743f,-23.9728048165f));
    Out4MomentsOptimized[0]+=0.035955884801f;
}

void SampleOptimized4MomentShadowMap(out float4 Out4Moments,
    SamplerState ShadowMapSampler,Texture2D Optimized4MomentShadowMap,
    float2 ShadowMapTexCoord)
{
    float4 _4MomentsOptimized=Optimized4MomentShadowMap.Sample(
        ShadowMapSampler,ShadowMapTexCoord
    );
    _4MomentsOptimized[0]-=0.035955884801f;
    Out4Moments=mul(_4MomentsOptimized,
        float4x4(0.2227744146f, 0.1549679261f, 0.1451988946f, 0.163127443f,
                 0.0771972861f,0.1394629426f,0.2120202157f,0.2591432266f,
                 0.7926986636f, 0.7963415838f, 0.7258694464f, 0.6539092497f,
                 0.0319417555f,-0.1722823173f,-0.2758014811f,-0.3376131734f));
}


float ComputeMSMShadowIntensity(float4 _4Moments,
    float FragmentDepth,float DepthBias,float MomentBias)
{
    float4 b=lerp(_4Moments,float4(0.5f,0.5f,0.5f,0.5f),MomentBias);
    float3 z;
    z[0]=FragmentDepth-DepthBias;
    float L32D22=mad(-b[0],b[1],b[2]);
    float D22=mad(-b[0],b[0], b[1]);
    float SquaredDepthVariance=mad(-b[1],b[1], b[3]);
    float D33D22=dot(float2(SquaredDepthVariance,-L32D22),
                     float2(D22,                  L32D22));
    float InvD22=1.0f/D22;
    float L32=L32D22*InvD22;
    float3 c=float3(1.0f,z[0],z[0]*z[0]);
    c[1]-=b.x;
    c[2]-=b.y+L32*c[1];
    c[1]*=InvD22;
    c[2]*=D22/D33D22;
    c[1]-=L32*c[2];
    c[0]-=dot(c.yz,b.xy);
    float p=c[1]/c[2];
    float q=c[0]/c[2];
    float r=sqrt((p*p*0.25f)-q);
    z[1]=-p*0.5f-r;
    z[2]=-p*0.5f+r;
    float4 Switch=
    	(z[2]<z[0])?float4(z[1],z[0],1.0f,1.0f):(
    	(z[1]<z[0])?float4(z[0],z[1],0.0f,1.0f):
    	float4(0.0f,0.0f,0.0f,0.0f));
    float Quotient=(Switch[0]*z[2]-b[0]*(Switch[0]+z[2])+b[1])
                  /((z[2]-Switch[1])*(z[0]-z[1]));
    return saturate(Switch[2]+Switch[3]*Quotient);
}
