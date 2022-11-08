﻿#include "stdafx.h"
#include "D2D1Support.h"
#include "Common.h"

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")

bool CD2D1Support::CheckSupport()
{
    static bool result = FunctionChecker::CheckFunctionExist(_T("D2d1.dll"), "D2D1CreateFactory");
    return result;
}

ID2D1Factory* CD2D1Support::GetFactory()
{
    static auto result = MakeStaticVariableWrapper<ID2D1Factory*>(
        [](ID2D1Factory** pp_factory)
        {
            *pp_factory = nullptr;

            ThrowIfFailed(::D2D1CreateFactory(
                              D2D1_FACTORY_TYPE_SINGLE_THREADED,
                              CD2D1Support::CREATION_OPTIONS,
                              pp_factory),
                          "Create D2D1 factory failed.");
        },
        [](ID2D1Factory** pp_factory)
        {
            auto* p_factory = *pp_factory;
            RELEASE_COM(p_factory);
        });
    return result.Get();
}

ID2D1Factory1* CD2D1Support1::GetFactory()
{
    static auto result = MakeStaticVariableWrapper<ID2D1Factory1*>(
        [](auto pp_factory)
        {
            *pp_factory = nullptr;

            ThrowIfFailed(::D2D1CreateFactory(
                              D2D1_FACTORY_TYPE_SINGLE_THREADED,
                              CD2D1Support::CREATION_OPTIONS,
                              pp_factory),
                          "Create D2D1 factory failed.");
        },
        [](auto pp_factory)
        {
            auto* p_factory = *pp_factory;
            RELEASE_COM(p_factory);
        });
    return result.Get();
}

void CD2D1Device::Recreate(Microsoft::WRL::ComPtr<IDXGIDevice> p_dxgi_device)
{
    ThrowIfFailed<CD2D1Exception>(
        CD2D1Support1::GetFactory()->CreateDevice(
            p_dxgi_device.Get(),
            &m_p_device),
        "CreateD2D1Device failed.");
}

auto CD2D1Device::GetStorage()
    -> std::shared_ptr<Storage>
{
    return m_resource_tracker.GetSharedResourceTrackerStorage();
}

bool CDWriteSupport::CheckSupport()
{
    static bool result = FunctionChecker::CheckFunctionExist(_T("Dwrite.dll"), "DWriteCreateFactory");
    return result;
}

IDWriteFactory* CDWriteSupport::GetFactory()
{
    static auto result = MakeStaticVariableWrapper<IDWriteFactory*>(
        [](IDWriteFactory** pp_factory)
        {
            *pp_factory = nullptr;
            ThrowIfFailed(::DWriteCreateFactory(
                              DWRITE_FACTORY_TYPE_SHARED,
                              __uuidof(IDWriteFactory),
                              reinterpret_cast<IUnknown**>(pp_factory)),
                          "Create DWrite factory failed.");
        },
        [](IDWriteFactory** pp_factory)
        {
            auto* p_factory = *pp_factory;
            RELEASE_COM(p_factory);
        });
    return result.Get();
}