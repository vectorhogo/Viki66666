#pragma once

#include <cstddef>
#include <cstdint>

#include "D3D11Structs.h"
#include "D3D11Types.h"

namespace d3d11
{

struct D3D11InputLayoutDesc {
    const char* semanticName;
    unsigned semanticIndex;
    Format format;
    unsigned inputSlot;
    unsigned alignedByteOffset;
};

constexpr unsigned kAppendAligned = 0xffffffff;

// Vertex layout used by all overlay shaders:
// float2 pos, float2 uv, float4 color, float depth.
struct OverlayVertex {
    float x, y;
    float u, v;
    float r, g, b, a;
    float depth;
};

static_assert(sizeof(OverlayVertex) == 36);

// Per-frame constant buffer for the vertex shader (VSConstants).
struct VsConstants {
    float mvp[4][4];
};

// BlurConstants.
struct BlurConstants {
    float texelSize[2];
    float direction[2];
    float radius;
    float pad[3];
};

// BloomConstants.
struct BloomConstants {
    float intensity;
    float srgb;
    float pad[2];
};

// DownsampleConstants / DownConstants / UpConstants.
// (16 bytes: float2 texelSize + implicit padding to cbuffer alignment.)
struct DownsampleConstants {
    float texelSize[2];
    float pad[2];
};

}
