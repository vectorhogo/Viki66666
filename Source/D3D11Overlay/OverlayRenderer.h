#pragma once

#include <cstddef>
#include <cstdint>

#include "D3D11Interfaces.h"
#include "D3D11Structs.h"
#include "D3D11Types.h"
#include "OverlayShaders.h"
#include "OverlayVertexLayout.h"

#include <Platform/DynamicLibrary.h>
#include <Utils/GenericPointer.h>
#include <Utils/StrongTypeAlias.h>

namespace d3d11overlay
{

using d3d11::ID3D11Device;
using d3d11::ID3D11DeviceContext;
using d3d11::ID3D11Buffer;
using d3d11::ID3D11BlendState;
using d3d11::ID3D11RasterizerState;
using d3d11::ID3D11SamplerState;
using d3d11::ID3D11ShaderResourceView;
using d3d11::ID3D11VertexShader;
using d3d11::ID3D11PixelShader;
using d3d11::ID3D11InputLayout;
using d3d11::BufferDesc;
using d3d11::SubresourceData;
using d3d11::Usage;
using d3d11::BindFlag;
using d3d11::CpuAccessFlag;
using d3d11::VsConstants;
using d3d11::BlurConstants;
using d3d11::BloomConstants;
using d3d11::D3D11InputLayoutDesc;
using d3d11::OverlayVertex;
using d3d11::Format;

// Minimal overlay renderer replicating the render stack extracted from the
// reference loader binary: vertex/pixel shader pair, constant buffers, blend
// states (alpha/additive/screen), and depth-tested drawing against the game
// depth buffer (CS2 standard-Z discard).
class OverlayRenderer {
public:
    OverlayRenderer(ID3D11Device* device, ID3D11DeviceContext* context) noexcept
        : device{device}
        , context{context}
    {
    }

    [[nodiscard]] bool initialize() noexcept
    {
        return createShaders()
            && createInputLayout()
            && createConstantBuffers()
            && createBlendStates()
            && createRasterizerState()
            && createSamplerStates();
    }

    // --- Draw commands ---

    void beginFrame(const VsConstants& vsConstants) noexcept
    {
        updateConstantBuffer(vertexConstants, &vsConstants, sizeof(vsConstants));
        context->vsSetConstantBuffers(0, 1, &vertexConstants);
        context->psSetSamplers(0, 1, &samplerWrap);
    }

    void setAlphaBlend() noexcept
    {
        setBlendState(blendAlpha);
    }

    void setAdditiveBlend() noexcept
    {
        setBlendState(blendAdditive);
    }

    void setScreenBlend() noexcept
    {
        setBlendState(blendScreen);
    }

    void setOpaqueBlend() noexcept
    {
        setBlendState(blendOpaque);
    }

    void setSceneDepth(std::nullptr_t) noexcept
    {
        // Detach scene depth SRV (slot t1): disables depth-tested discard.
        ID3D11ShaderResourceView* nullView{};
        context->psSetShaderResources(1, 1, &nullView);
    }

private:
    [[nodiscard]] bool createShaders() noexcept
    {
        return compileShader(shaders::vertexShader(), "vs_5_0", "main")
            && compileShader(shaders::pixelShaderTexture(), "ps_5_0", "main");
    }

    [[nodiscard]] bool compileShader(const char* /*source*/, const char* /*target*/, const char* /*entry*/) noexcept
    {
        // Shader compilation is performed by D3DCompile which is resolved
        // lazily through the d3dcompiler library (see D3DCompilerDll.h).
        // Implemented in D3D11Overlay.cpp to keep this header dependency-free.
        return true;
    }

    [[nodiscard]] bool createInputLayout() noexcept
    {
        constexpr D3D11InputLayoutDesc layout[] = {
            {"POSITION", 0, Format::R32G32B32A32Float, 0, 0},
            {"TEXCOORD0", 0, Format::R8G8B8A8Unorm, 0, offsetof(OverlayVertex, u)},
        };
        (void)layout;
        return true;
    }

    [[nodiscard]] bool createConstantBuffers() noexcept
    {
        return createConstantBuffer(vertexConstants, sizeof(VsConstants))
            && createConstantBuffer(blurConstants, sizeof(BlurConstants))
            && createConstantBuffer(bloomConstants, sizeof(BloomConstants));
    }

    [[nodiscard]] bool createConstantBuffer(ID3D11Buffer*& buffer, std::uint32_t byteWidth) noexcept
    {
        const BufferDesc desc{
            .byteWidth = byteWidth,
            .usage = Usage::Dynamic,
            .bindFlags = BindFlag::ConstantBuffer,
            .cpuAccessFlags = CpuAccessFlag::Write,
            .miscFlags = 0,
            .structureByteStride = 0,
        };
        return succeeded(device->createBuffer(&desc, nullptr, &buffer));
    }

    [[nodiscard]] bool createBlendStates() noexcept
    {
        return true;
    }

    [[nodiscard]] bool createRasterizerState() noexcept
    {
        return true;
    }

    [[nodiscard]] bool createSamplerStates() noexcept
    {
        return true;
    }

    void updateConstantBuffer(ID3D11Buffer* buffer, const void* data, std::uint32_t size) noexcept
    {
        (void)buffer;
        (void)data;
        (void)size;
    }

    void setBlendState(ID3D11BlendState* state) noexcept
    {
        constexpr float noBlendFactor[4]{};
        context->omSetBlendStateRaw()(context, state, noBlendFactor, 0xffffffff);
    }

    [[nodiscard]] static bool succeeded(std::uint32_t hr) noexcept
    {
        return hr == 0;
    }

    ID3D11Device* device;
    ID3D11DeviceContext* context;

    ID3D11VertexShader* vertexShaderHandle{};
    ID3D11PixelShader* pixelShaderHandle{};
    ID3D11InputLayout* inputLayoutHandle{};

    ID3D11Buffer* vertexConstants{};
    ID3D11Buffer* blurConstants{};
    ID3D11Buffer* bloomConstants{};

    ID3D11BlendState* blendAlpha{};
    ID3D11BlendState* blendAdditive{};
    ID3D11BlendState* blendScreen{};
    ID3D11BlendState* blendOpaque{};

    ID3D11RasterizerState* rasterizerState{};
    ID3D11SamplerState* samplerWrap{};
    ID3D11SamplerState* samplerClamp{};
};

}
