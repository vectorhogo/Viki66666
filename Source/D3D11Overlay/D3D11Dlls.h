#pragma once

#include "D3D11Types.h"

#include <Platform/DynamicLibrary.h>

struct D3D11Dll : DynamicLibrary {
    D3D11Dll() noexcept
        : DynamicLibrary{"d3d11.dll"}
    {
    }

    [[nodiscard]] d3d11::D3D11CreateDeviceAndSwapChain* createDeviceAndSwapChain() const noexcept
    {
        return getFunctionAddress("D3D11CreateDeviceAndSwapChain").as<d3d11::D3D11CreateDeviceAndSwapChain*>();
    }
};

struct D3DCompilerDll : DynamicLibrary {
    D3DCompilerDll() noexcept
        : DynamicLibrary{"D3DCOMPILER_47.dll"}
    {
    }

    [[nodiscard]] d3d11::D3DCompile* compile() const noexcept
    {
        return getFunctionAddress("D3DCompile").as<d3d11::D3DCompile*>();
    }
};
