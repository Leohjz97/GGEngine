cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

cbuffer ReflectionBuffer : register(b1)
{
    matrix reflectionMatrix;
}

cbuffer PowerBuffer : register(b2)
{
    float power;
}

Texture2D shaderTexture : register(t0);
Texture2D reflectionTexture : register(t1);
SamplerState SampleType : register(s0);

struct VS_INPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
    float4 reflectionPosition : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    matrix reflectProjectWorld;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.texCoord = input.texCoord;

    reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
    reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

    output.reflectionPosition = mul(input.position, reflectProjectWorld);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 textureColor;
    float2 reflectTexCoord;
    float4 reflectionColor;
    float4 color;

    textureColor = shaderTexture.Sample(SampleType, input.texCoord);

    reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

    reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

    color = lerp(textureColor, reflectionColor, power);

    return color;
}