cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}

cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float4 lightDiffuse;
    float padding;
}

Texture2D shaderMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, world);
    output.position = mul(input.position, view);
    output.position = mul(input.position, projection);
    
    output.texCoord = input.texCoord;
    
    output.normal = mul(input.normal, (float3x3)world);
    output.normal = normalize(output.normal);
    
    return output;
}

float4 PS(VS_OUTPUT input):SV_Target
{
    float4 finalColor;
    
    finalColor = shaderMap.Sample(textureSampler, input.texCoord) * lightDiffuse;
    finalColor.a = 1.0f;
    
    float intensity = dot(normalize(lightDirection), input.normal);

    if (intensity < 0)
    {
        intensity = 0;
    }
    
    if (intensity > 0.95)
    {
        finalColor = float4(1.0, 1, 1, 1.0) * finalColor;
    }
    else if (intensity > 0.5)
    {
        finalColor = float4(0.7, 0.7, 0.7, 1.0) * finalColor;
    }
    else if (intensity > 0.05)
    {
        finalColor = float4(0.35, 0.35, 0.35, 1.0) * finalColor;
    }
    else
    {
        finalColor = float4(0.2, 0.2, 0.2, 1.0) * finalColor;
    }
    
    return finalColor;
}