#pragma once

#include <cstddef>
#include <cstdint>

#include "D3D11Structs.h"
#include "D3D11Types.h"

namespace d3d11
{

// Minimal ABI-compatible COM interface definitions.
// Named virtual methods must sit at the exact d3d11.h vtable slot;
// slots in between are covered by unusedPad<N> declarations.
// All slot numbers were cross-checked against the standard d3d11.h.

struct IUnknown {
    virtual std::uint32_t queryInterface(const void* riid, void** object) = 0;   // 0
    virtual std::uint32_t addRef() = 0;                                          // 1
    virtual std::uint32_t release() = 0;                                         // 2
};

struct ID3D11DeviceChild : IUnknown {
    virtual void pad3() = 0;                                                     // 3  GetDevice
    virtual void pad4() = 0;                                                     // 4  GetPrivateData
    virtual void pad5() = 0;                                                     // 5  SetPrivateData
    virtual void pad6() = 0;                                                     // 6  SetPrivateData1
};

struct ID3D11Resource : ID3D11DeviceChild {
    virtual void pad7() = 0;                                                     // 7  GetType
    virtual void pad8() = 0;                                                     // 8  SetEvictionPriority
    virtual void pad9() = 0;                                                     // 9  GetEvictionPriority
};

struct ID3D11Buffer : ID3D11Resource {};

struct ID3D11Texture2D : ID3D11Resource {};

struct ID3D11ShaderResourceView : ID3D11DeviceChild {};

struct ID3D11RenderTargetView : ID3D11DeviceChild {};

struct ID3D11DepthStencilView : ID3D11DeviceChild {};

struct ID3D11InputLayout : ID3D11DeviceChild {};

struct ID3D11VertexShader : ID3D11DeviceChild {};

struct ID3D11PixelShader : ID3D11DeviceChild {};

struct ID3D11BlendState : ID3D11DeviceChild {};

struct ID3D11RasterizerState : ID3D11DeviceChild {};

struct ID3D11DepthStencilState : ID3D11DeviceChild {};

struct ID3D11SamplerState : ID3D11DeviceChild {};

// ID3D11Device derives directly from IUnknown (no ID3D11DeviceChild).
// d3d11.h slot order: 3 CreateBuffer, 4 CreateTexture1D, 5 CreateTexture2D, ...
struct ID3D11Device : IUnknown {
    virtual std::uint32_t createBuffer(const BufferDesc* desc, const SubresourceData* initData, ID3D11Buffer** buffer) = 0;              // 3
    virtual void pad4() = 0;                                                                                                               // 4  CreateTexture1D
    virtual std::uint32_t createTexture2D(const Texture2DDesc* desc, const SubresourceData* initData, ID3D11Texture2D** texture) = 0;      // 5
};

// ID3D11DeviceContext slot order (d3d11.h):
// 3 VSSetConstantBuffers, 4 PSSetShaderResources, 5 PSSetShader, 6 PSSetSamplers,
// 7 VSSetShader, 8 DrawIndexed, 9 Draw, 10 Map, 11 Unmap, 12 PSSetConstantBuffers,
// 13 IASetInputLayout, 14 IASetVertexBuffers, 15 IASetIndexBuffer, 16 DrawIndexedInstanced,
// 17 DrawInstanced, 18 VSSetShaderResources, 19 VSSetSamplers, 20 GSSetConstantBuffers,
// ... 22 OMSetBlendState (padding between is not representable with consecutive
// pure virtuals, so OMSetBlendState and later slots must be dispatched through
// the raw vtable — see vtableSlot()/callSlot() below).
struct ID3D11DeviceContext : IUnknown {
    virtual void vsSetConstantBuffers(unsigned startSlot, unsigned numBuffers, ID3D11Buffer* const* buffers) = 0;                         // 3
    virtual void psSetShaderResources(unsigned startSlot, unsigned numViews, ID3D11ShaderResourceView* const* views) = 0;                  // 4
    virtual void pad5() = 0;                                                                                                               // 5  PSSetShader
    virtual void psSetSamplers(unsigned startSlot, unsigned numSamplers, ID3D11SamplerState* const* samplers) = 0;                         // 6
    virtual void pad7() = 0;                                                                                                               // 7  VSSetShader
    virtual void pad8() = 0;                                                                                                               // 8  DrawIndexed
    virtual void draw(unsigned vertexCount, unsigned startVertex) = 0;                                                                     // 9

    // Slots >= 10 are reached through raw vtable dispatch:
    using OmSetBlendStateFn = void(ID3D11DeviceContext*, ID3D11BlendState*, const float*, unsigned);
    [[nodiscard]] OmSetBlendStateFn* omSetBlendStateRaw() const noexcept
    {
        return vtableSlot<OmSetBlendStateFn, 22>();
    }

    using DrawIndexedFn = void(ID3D11DeviceContext*, unsigned, unsigned, unsigned, unsigned, unsigned);
    [[nodiscard]] DrawIndexedFn* drawIndexedRaw() const noexcept
    {
        return vtableSlot<DrawIndexedFn, 8>();
    }

    using IaSetInputLayoutFn = void(ID3D11DeviceContext*, ID3D11InputLayout*);
    [[nodiscard]] IaSetInputLayoutFn* iaSetInputLayoutRaw() const noexcept
    {
        return vtableSlot<IaSetInputLayoutFn, 13>();
    }

    using IaSetVertexBuffersFn = void(ID3D11DeviceContext*, unsigned, unsigned, ID3D11Buffer* const*, const unsigned*, const unsigned*);
    [[nodiscard]] IaSetVertexBuffersFn* iaSetVertexBuffersRaw() const noexcept
    {
        return vtableSlot<IaSetVertexBuffersFn, 14>();
    }

private:
    template <typename Fn, unsigned Slot>
    [[nodiscard]] Fn* vtableSlot() const noexcept
    {
        void* const* vmt = *reinterpret_cast<void* const* const*>(this);
        return reinterpret_cast<Fn*>(vmt[Slot]);
    }
};

// IDXGISwapChain slot order (dxgi.h): 8 Present, 9 GetBuffer, 14 ResizeBuffers.
struct IDXGISwapChain : IUnknown {
    virtual void pad3() = 0;                                                                                                               // 3  SetPrivateData
    virtual void pad4() = 0;                                                                                                               // 4  GetPrivateData
    virtual void pad5() = 0;                                                                                                               // 5  GetParent
    virtual void pad6() = 0;                                                                                                               // 6  GetDesc
    virtual void pad7() = 0;                                                                                                               // 7  ResizeBuffers
    virtual std::uint32_t present(unsigned syncInterval, unsigned flags) = 0;                                                              // 8
    virtual std::uint32_t getBuffer(unsigned buffer, const void* riid, void** surface) = 0;                                                // 9
};

}
