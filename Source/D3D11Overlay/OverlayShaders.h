#pragma once

namespace d3d11overlay::shaders
{

consteval auto vertexShader()
{
    return R"(
cbuffer VSConstants : register(b0)
{
    float4x4 mvp;
};

struct VSIn
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float depth : DEPTH;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

VSOut main(VSIn input)
{
    VSOut output;
    output.pos = mul(mvp, float4(input.pos.xy, 0.0f, 1.0f));
    output.pos.z = input.depth;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}
)";
}

consteval auto pixelShaderTexture()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 tex = tex0.Sample(samp0, input.uv);
    return tex * input.color;
}
)";
}

consteval auto pixelShaderNoTex()
{
    return R"(
struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    return input.color;
}
)";
}

consteval auto pixelShaderFont()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float coverage = tex0.Sample(samp0, input.uv).r;
    // Perceptual stem-weight restore for linear FreeType coverage (approx of
    // gamma-correct blending; tune the exponent in [1.2, 1.8] to taste).
    coverage = pow(coverage, 1.0 / 1.4);
    return float4(input.color.rgb, input.color.a * coverage);
}
)";
}

consteval auto pixelShaderBlitSrgb()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float3 srgb_to_linear(float3 c)
{
    float3 lo = c / 12.92;
    float3 hi = pow((c + 0.055) / 1.055, 2.4);
    return lerp(lo, hi, step(0.04045, c));
}

float4 main(PSIn input) : SV_TARGET
{
    float4 tex = tex0.Sample(samp0, input.uv);
    float4 result = tex * input.color;
    result.rgb = srgb_to_linear(saturate(result.rgb));
    return result;
}
)";
}

consteval auto pixelShaderDepthColor()
{
    return R"(
Texture2D<float> scene_depth : register(t1);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float scene_z = scene_depth.Load(int3(int2(input.pos.xy), 0));
    if (input.pos.z > scene_z + 0.05f) discard;   // CS2 standard-Z (writeup).
    return input.color;
}
)";
}

consteval auto pixelShaderDepthColorMs()
{
    return R"(
Texture2DMS<float> scene_depth : register(t1);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float scene_z = scene_depth.Load(int2(input.pos.xy), 0);
    if (input.pos.z > scene_z + 0.05f) discard;   // CS2 standard-Z (writeup).
    return input.color;
}
)";
}

consteval auto pixelShaderDepthTexture()
{
    return R"(
Texture2D<float> scene_depth : register(t1);
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float scene_z = scene_depth.Load(int3(int2(input.pos.xy), 0));
    if (input.pos.z > scene_z + 0.05f) discard;   // CS2 standard-Z (writeup).
    return tex0.Sample(samp0, input.uv) * input.color;
}
)";
}

consteval auto pixelShaderDepthTextureMs()
{
    return R"(
Texture2DMS<float> scene_depth : register(t1);
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float scene_z = scene_depth.Load(int2(input.pos.xy), 0);
    if (input.pos.z > scene_z + 0.05f) discard;   // CS2 standard-Z (writeup).
    return tex0.Sample(samp0, input.uv) * input.color;
}
)";
}

consteval auto pixelShaderDepthFont()
{
    return R"(
Texture2D<float> scene_depth : register(t1);
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float scene_z = scene_depth.Load(int3(int2(input.pos.xy), 0));
    if (input.pos.z > scene_z + 0.05f) discard;   // CS2 standard-Z (writeup).
    float coverage = tex0.Sample(samp0, input.uv).r;
    // Perceptual stem-weight restore for linear FreeType coverage (approx of
    // gamma-correct blending; tune the exponent in [1.2, 1.8] to taste).
    coverage = pow(coverage, 1.0 / 1.4);
    return float4(input.color.rgb, input.color.a * coverage);
}
)";
}

consteval auto pixelShaderDepthFontMs()
{
    return R"(
Texture2DMS<float> scene_depth : register(t1);
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    float scene_z = scene_depth.Load(int2(input.pos.xy), 0);
    if (input.pos.z > scene_z + 0.05f) discard;   // CS2 standard-Z (writeup).
    float coverage = tex0.Sample(samp0, input.uv).r;
    // Perceptual stem-weight restore for linear FreeType coverage (approx of
    // gamma-correct blending; tune the exponent in [1.2, 1.8] to taste).
    coverage = pow(coverage, 1.0 / 1.4);
    return float4(input.color.rgb, input.color.a * coverage);
}
)";
}

consteval auto pixelShaderBlur()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

cbuffer BlurConstants : register(b0)
{
    float2 texel_size;
    float2 direction;
    float radius;
    float3 _pad;
};

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PSIn input) : SV_TARGET
{
    static const float w[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
    float2 step = direction * texel_size * (radius / 4.0);
    float4 sum = tex0.Sample(samp0, input.uv) * w[0];
    [unroll]
    for (int k = 1; k < 5; ++k)
    {
        float2 off = step * (float)k;
        sum += tex0.Sample(samp0, input.uv + off) * w[k];
        sum += tex0.Sample(samp0, input.uv - off) * w[k];
    }
    return sum;
}
)";
}

consteval auto pixelShaderBloom()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

cbuffer BloomConstants : register(b0)
{
    float intensity;
    float srgb;
    float2 _pad;
};

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float3 srgb_to_linear(float3 c)
{
    float3 lo = c / 12.92;
    float3 hi = pow((c + 0.055) / 1.055, 2.4);
    return lerp(lo, hi, step(0.04045, c));
}

float4 main(PSIn input) : SV_TARGET
{
    float4 c = tex0.Sample(samp0, input.uv) * intensity;
    if (srgb > 0.5)
        c.rgb = srgb_to_linear(saturate(c.rgb));
    return c;
}
)";
}

consteval auto pixelShaderBrightpass()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

cbuffer DownsampleConstants : register(b0)
{
    float2 texel_size;
    float2 _p0;
    float _p1;
    float3 _p2;
};

struct PSIn { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; float4 color : COLOR0; };

float kw(float3 c) { return 1.0 / (1.0 + max(c.r, max(c.g, c.b))); }

float4 main(PSIn input) : SV_TARGET
{
    float2 t = texel_size;
    float3 a = tex0.Sample(samp0, input.uv + float2(-2.0, -2.0) * t).rgb;
    float3 b = tex0.Sample(samp0, input.uv + float2( 0.0, -2.0) * t).rgb;
    float3 c = tex0.Sample(samp0, input.uv + float2( 2.0, -2.0) * t).rgb;
    float3 d = tex0.Sample(samp0, input.uv + float2(-2.0,  0.0) * t).rgb;
    float3 e = tex0.Sample(samp0, input.uv + float2( 0.0,  0.0) * t).rgb;
    float3 f = tex0.Sample(samp0, input.uv + float2( 2.0,  0.0) * t).rgb;
    float3 g = tex0.Sample(samp0, input.uv + float2(-2.0,  2.0) * t).rgb;
    float3 h = tex0.Sample(samp0, input.uv + float2( 0.0,  2.0) * t).rgb;
    float3 ii = tex0.Sample(samp0, input.uv + float2( 2.0,  2.0) * t).rgb;
    float3 j = tex0.Sample(samp0, input.uv + float2(-1.0, -1.0) * t).rgb;
    float3 k = tex0.Sample(samp0, input.uv + float2( 1.0, -1.0) * t).rgb;
    float3 l = tex0.Sample(samp0, input.uv + float2(-1.0,  1.0) * t).rgb;
    float3 m = tex0.Sample(samp0, input.uv + float2( 1.0,  1.0) * t).rgb;

    float3 g0 = (j + k + l + m) * 0.25; // inner quad (center)
    float3 g1 = (a + b + d + e) * 0.25; // top-left
    float3 g2 = (b + c + e + f) * 0.25; // top-right
    float3 g3 = (d + e + g + h) * 0.25; // bottom-left
    float3 g4 = (e + f + h + ii) * 0.25; // bottom-right

    float w0 = 0.5   * kw(g0);
    float w1 = 0.125 * kw(g1);
    float w2 = 0.125 * kw(g2);
    float w3 = 0.125 * kw(g3);
    float w4 = 0.125 * kw(g4);

    float3 col = (g0 * w0 + g1 * w1 + g2 * w2 + g3 * w3 + g4 * w4) / (w0 + w1 + w2 + w3 + w4);

    float lum = max(col.r, max(col.g, col.b));
    col *= smoothstep(0.04, 0.20, lum);

    float mx = max(col.r, max(col.g, col.b));
    float mn = min(col.r, min(col.g, col.b));
    float sat = mx > 0.0 ? (mx - mn) / mx : 0.0;
    col *= 1.0 - 0.7 * mx * (1.0 - sat);

    return float4(col, 1.0);
}
)";
}

consteval auto pixelShaderBloomDown()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

cbuffer DownConstants : register(b0) { float2 texel_size; float2 _p0; float _p1; float3 _p2; };

struct PSIn { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; float4 color : COLOR0; };

float kw(float3 c) { return 1.0 / (1.0 + max(c.r, max(c.g, c.b))); }

float4 main(PSIn i) : SV_TARGET
{
    float2 t = texel_size;
    float4 a = tex0.Sample(samp0, i.uv + float2(-t.x, -t.y));
    float4 b = tex0.Sample(samp0, i.uv + float2( t.x, -t.y));
    float4 c = tex0.Sample(samp0, i.uv + float2(-t.x,  t.y));
    float4 d = tex0.Sample(samp0, i.uv + float2( t.x,  t.y));

    float wa = kw(a.rgb), wb = kw(b.rgb), wc = kw(c.rgb), wd = kw(d.rgb);
    return (a * wa + b * wb + c * wc + d * wd) / (wa + wb + wc + wd);
}
)";
}

consteval auto pixelShaderBloomUp()
{
    return R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

cbuffer UpConstants : register(b0) { float2 texel_size; float2 _p0; float _p1; float3 _p2; };

struct PSIn { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; float4 color : COLOR0; };

float4 main(PSIn i) : SV_TARGET
{
    float2 t = texel_size;
    float4 s = tex0.Sample(samp0, i.uv) * 4.0;
    s += tex0.Sample(samp0, i.uv + float2(-t.x, 0.0)) * 2.0;
    s += tex0.Sample(samp0, i.uv + float2( t.x, 0.0)) * 2.0;
    s += tex0.Sample(samp0, i.uv + float2(0.0, -t.y)) * 2.0;
    s += tex0.Sample(samp0, i.uv + float2(0.0,  t.y)) * 2.0;
    s += tex0.Sample(samp0, i.uv + float2(-t.x, -t.y));
    s += tex0.Sample(samp0, i.uv + float2( t.x, -t.y));
    s += tex0.Sample(samp0, i.uv + float2(-t.x,  t.y));
    s += tex0.Sample(samp0, i.uv + float2( t.x,  t.y));
    return s / 16.0;
}
)";
}

}
