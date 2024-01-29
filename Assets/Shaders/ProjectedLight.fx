// Description: Simple shader that does transform.
cbuffer MatrixBuffer : register(b0)
{
    matrix worldM;
    matrix viewM;
    matrix projM;
    matrix viewM2;
    matrix projM2;
}

cbuffer LightPositionBuffer : register(b1)
{
    float3 lightPosition;
    float padding;
};

cbuffer LightBuffer : register(b2)
{
    float4 ambientColor;
    float4 diffuserColor;
};

Texture2D shaderTexture : register(t0);
Texture2D projectionTexture : register(t1);
SamplerState sampleType : register(s0);

struct VS_INPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float4 viewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float4 viewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPosition;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldM);
    output.position = mul(output.position, viewM);
    output.position = mul(output.position, projM);

    output.viewPosition = mul(input.position, worldM);
    output.viewPosition = mul(output.viewPosition, viewM2);
    output.viewPosition = mul(output.viewPosition, projM2);

    output.texCoord = input.texCoord;

    output.normal = mul(input.normal, (float3x3)worldM);

    output.normal = normalize(output.normal);

    worldPosition = mul(input.position, worldM);

    output.lightPos = lightPosition.xyz - worldPosition.xyz;

    output.lightPos = normalize(output.lightPos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float brightness;
    float4 finalColor;
    float lightntensity;
    float4 textureColor;
    float2 projectTextcoord;
    float4 projectionColor;
    
    brightness = 1.5f;
    lightntensity = saturate(dot(input.normal, input.lightPos));
    
    if (lightntensity > 0.0f)
    {
        finalColor = (diffuserColor * lightntensity) * brightness;
    }
    
    textureColor = shaderTexture.Sample(sampleType, input.texCoord);
    
    projectionColor.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectionColor.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
    
    if ((saturate(projectTextcoord.x) == projectTextcoord.x) && (saturate(projectTextcoord.y) == projectTextcoord.y))
    {
        projectionColor = projectionTexture.Sample(sampleType, projectTextcoord);
        finalColor = saturate((finalColor * projectionColor * textureColor) + (ambientColor * textureColor));
    }
    else
    {
        finalColor = ambientColor * textureColor;
    }

    return finalColor;
}