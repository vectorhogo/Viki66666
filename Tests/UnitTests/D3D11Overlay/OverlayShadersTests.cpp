#include <string_view>

#include <gtest/gtest.h>

#include <D3D11Overlay/OverlayShaders.h>
#include <D3D11Overlay/OverlayVertexLayout.h>

namespace
{

consteval bool contains(const char* haystack, std::string_view needle)
{
    std::string_view haystackView{haystack};
    return haystackView.find(needle) != std::string_view::npos;
}

TEST(OverlayVertexLayoutTest, VertexHas36Bytes) {
    static_assert(sizeof(d3d11::OverlayVertex) == 36);
    SUCCEED();
}

TEST(OverlayVertexLayoutTest, ConstantBufferSizesAreMultipleOf16) {
    // D3D11 requires constant buffer sizes to be multiples of 16 bytes.
    static_assert(sizeof(d3d11::VsConstants) == 64);
    static_assert(sizeof(d3d11::BlurConstants) == 32);
    static_assert(sizeof(d3d11::BloomConstants) == 16);
    static_assert(sizeof(d3d11::DownsampleConstants) == 16);
    SUCCEED();
}

TEST(OverlayShadersTest, DepthShadersContainStandardZDiscard) {
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthColor(), "if (input.pos.z > scene_z + 0.05f) discard;"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthFont(), "if (input.pos.z > scene_z + 0.05f) discard;"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthFontMs(), "if (input.pos.z > scene_z + 0.05f) discard;"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthTexture(), "if (input.pos.z > scene_z + 0.05f) discard;"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthTextureMs(), "if (input.pos.z > scene_z + 0.05f) discard;"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthColorMs(), "if (input.pos.z > scene_z + 0.05f) discard;"));
    SUCCEED();
}

TEST(OverlayShadersTest, FontShadersApplyStemWeightRestore) {
    static_assert(contains(d3d11overlay::shaders::pixelShaderFont(), "1.0 / 1.4"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthFont(), "1.0 / 1.4"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderDepthFontMs(), "1.0 / 1.4"));
    SUCCEED();
}

TEST(OverlayShadersTest, BloomShadersUseSrgbToLinear) {
    static_assert(contains(d3d11overlay::shaders::pixelShaderBloom(), "srgb_to_linear"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderBlitSrgb(), "srgb_to_linear"));
    SUCCEED();
}

TEST(OverlayShadersTest, BrightpassUsesKarisAverage) {
    static_assert(contains(d3d11overlay::shaders::pixelShaderBrightpass(), "float kw(float3 c)"));
    static_assert(contains(d3d11overlay::shaders::pixelShaderBloomDown(), "float kw(float3 c)"));
    SUCCEED();
}

TEST(OverlayShadersTest, VertexShaderPassesDepth) {
    static_assert(contains(d3d11overlay::shaders::vertexShader(), "output.pos.z = input.depth;"));
    SUCCEED();
}

}
