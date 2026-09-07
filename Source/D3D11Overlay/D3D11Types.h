#pragma once

#include <cstddef>
#include <cstdint>

#include <Utils/GenericPointer.h>

namespace d3d11
{

struct IUnknown;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11BlendState;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

using D3D11CreateDeviceAndSwapChain = long(std::uint32_t driverType, ID3D11Device* device, unsigned flags, const void* featureLevels, unsigned numFeatureLevels, unsigned sdkVersion, const void* swapChainDesc, IDXGISwapChain* swapChain, ID3D11Device** deviceCreated, void* featureLevel, ID3D11DeviceContext** immediateContext);

using D3DCompile = long(const void* srcData, std::size_t srcDataSize, const char* sourceName, const void* defines, const void* includeHandler, const char* entryPoint, const char* target, unsigned flags1, unsigned flags2, void** codeBlob, void** errorBlob);

namespace FeatureLevel
{
    constexpr unsigned k11_0 = 0xb000;
}

constexpr unsigned kSdkVersion = 7;

enum class DriverType : std::uint32_t
{
    Hardware = 1,
    Warp = 5,
};

enum class Format : std::uint32_t
{
    Unknown = 0,
    R32G32B32A32Float = 2,
    R8G8B8A8Unorm = 28,
    R32G32B32A32Typeless = 91,
    D32Float = 40,
    D24UnormS8Uint = 45,
};

enum class CpuAccessFlag : std::uint32_t
{
    None = 0,
    Write = 0x10000,
};

enum class BindFlag : std::uint32_t
{
    None = 0,
    VertexBuffer = 0x2,
    IndexBuffer = 0x4,
    ConstantBuffer = 0x8,
    ShaderResource = 0x80,
    RenderTarget = 0x20,
    DepthStencil = 0x10,
};

enum class Usage : std::uint32_t
{
    Default = 0,
    Immutable = 1,
    Dynamic = 2,
    Staging = 3,
};

enum class BlendOp : std::uint32_t
{
    Add = 1,
    Subtract = 2,
    RevSubtract = 3,
    Min = 4,
    Max = 5,
};

enum class Blend : std::uint32_t
{
    Zero = 1,
    One = 2,
    SrcColor = 3,
    InvSrcColor = 4,
    SrcAlpha = 5,
    InvSrcAlpha = 6,
    DestAlpha = 7,
    InvDestAlpha = 8,
    DestColor = 9,
    InvDestColor = 10,
};

enum class CullMode : std::uint32_t
{
    None = 1,
    Front = 2,
    Back = 3,
};

enum class FillMode : std::uint32_t
{
    Wireframe = 2,
    Solid = 3,
};

enum class ComparisonFunc : std::uint32_t
{
    Never = 1,
    Less = 2,
    Always = 8,
};

enum class TextureAddressMode : std::uint32_t
{
    Wrap = 1,
    Clamp = 3,
};

struct BlendDesc;

struct Rgba
{
    float r{}, g{}, b{}, a{};
};

struct Point
{
    float x{}, y{};
};

constexpr float kDefaultDepth = 0.0f;
constexpr float kOnTopDepth = 1.0f;

}
