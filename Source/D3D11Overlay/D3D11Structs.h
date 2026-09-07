#pragma once

#include <cstddef>
#include <cstdint>

#include "D3D11Types.h"

namespace d3d11
{

// Sub-resource init data for ID3D11Device::CreateBuffer / UpdateSubresource.
struct SubresourceData {
    const void* systemMemory;
    std::uint32_t sysMemPitch;
    std::uint32_t sysMemSlicePitch;
};

// Buffer description (CreateBuffer).
struct BufferDesc {
    std::uint32_t byteWidth;
    Usage usage;
    BindFlag bindFlags;
    CpuAccessFlag cpuAccessFlags;
    std::uint32_t miscFlags;
    std::uint32_t structureByteStride;
};

struct DxgiSampleDesc {
    std::uint32_t count;
    std::uint32_t quality;
};

struct DxgiRational {
    std::uint32_t numerator;
    std::uint32_t denominator;
};

// Texture description (CreateTexture2D).
struct Texture2DDesc {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t mipLevels;
    std::uint32_t arraySize;
    Format format;
    DxgiSampleDesc sampleDesc;
    Usage usage;
    BindFlag bindFlags;
    CpuAccessFlag cpuAccessFlags;
    std::uint32_t miscFlags;
};

struct DxgiModeDesc {
    std::uint32_t width;
    std::uint32_t height;
    DxgiRational refreshRate;
    Format format;
    std::uint32_t scanlineOrdering;
    std::uint32_t scaling;
};

struct SwapChainDesc {
    DxgiModeDesc bufferDesc;
    DxgiSampleDesc sampleDesc;
    std::uint32_t bufferUsage;
    std::uint32_t outputWindow;
    std::uint32_t windowed;
    std::uint32_t swapEffect;
    std::uint32_t bufferCount;
    std::uint32_t flags;
};

}
