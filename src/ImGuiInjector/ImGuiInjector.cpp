#include "ImGuiInjector/ImGuiInjector.hpp"

#include <iostream>
#include <windows.h>

#include "Helpers/Helpers.hpp"
#include "imgui/imgui.h"
#include "kiero/kiero.h"

#pragma comment (lib, "BaseModels.lib")

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

#if KIERO_INCLUDE_D3D9
# include "kiero/examples/imgui/impl/d3d9_impl.h"
#endif

#if KIERO_INCLUDE_D3D10
# include "kiero/examples/imgui/impl/d3d10_impl.h"
#endif

#if KIERO_INCLUDE_D3D11
# include "kiero/examples/imgui/impl/d3d11_impl.h"
#endif

#if KIERO_INCLUDE_D3D12
#endif

#if KIERO_INCLUDE_OPENGL
#endif

#if KIERO_INCLUDE_VULKAN
#endif

#if !KIERO_USE_MINHOOK
# error "The example requires that minhook be enabled!"
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGuiInjector::Get().WndProcHandler(hWnd, msg, wParam, lParam)) {
    	return true;
    }
    return CallWindowProc(ImGuiInjector::Get().GetPreviousWndProc(), hWnd, msg, wParam, lParam);
};

ImGuiInjector& ImGuiInjector::Get() {
    static ImGuiInjector instance;
    return instance;
}

ImGuiInjector::ImGuiInjector() {
    ImGuiInjector::mMenus.clear();
    ImGuiInjector::Init();
}

int ImGuiInjector::Init() {
    if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
    {
        switch (kiero::getRenderType())
        {
#if KIERO_INCLUDE_D3D9
        case kiero::RenderType::D3D9:
            std::cout << "Kiero DX9 initialised\n";
            impl::d3d9::init();
            break;
#endif
#if KIERO_INCLUDE_D3D10
        case kiero::RenderType::D3D10:
            std::cout << "Kiero DX10 initialised\n";
            impl::d3d10::init();
            break;
#endif
#if KIERO_INCLUDE_D3D11
        case kiero::RenderType::D3D11:
            std::cout << "Kiero DX11 initialised\n";
            impl::d3d11::init();
            break;
#endif
        case kiero::RenderType::D3D12:
            // TODO: D3D12 implementation?
            break;
        case kiero::RenderType::OpenGL:
            // TODO: OpenGL implementation?
            break;
        case kiero::RenderType::Vulkan:
            // TODO: Vulkan implementation?
            break;
        }
        return 1;
    }
    
    mLogger.LogOut("Failed to initialize ImGui");

    return 0;
}

void ImGuiInjector::SetWndProcHook(HWND hWnd) {
    ImGuiInjector::mWindowHandle = hWnd;
    ImGuiInjector::mWndProcPtr = reinterpret_cast<LONG_PTR>(WndProc);
    ImGuiInjector::mPreviousWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(ImGuiInjector::mWindowHandle, GWLP_WNDPROC, ImGuiInjector::mWndProcPtr));
}

HWND ImGuiInjector::GetWindowHandle() {
    return ImGuiInjector::mWindowHandle;
}

WNDPROC ImGuiInjector::GetPreviousWndProc() {
    return ImGuiInjector::mPreviousWndProc;
}

bool ImGuiInjector::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    for (auto it = mMenus.begin(); it != mMenus.end(); it++) {
        ImGuiMenu* menu = *(it._Ptr);
        menu->MessageHandler(hWnd, msg, wParam, lParam);
    }
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }
    return false;
}

void ImGuiInjector::AddMenu(ImGuiMenu* pMenu) {
    ImGuiInjector::mMenus.push_back(pMenu);
}

void ImGuiInjector::RunMenus() {
    bool drawImGuiCursor = false;
    for (auto it = mMenus.begin(); it != mMenus.end(); it++) {
        ImGuiMenu* menu = *(it._Ptr);
        if (menu->IsOpen()) {
            drawImGuiCursor = true;
            menu->Loop();
        }
    }
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = drawImGuiCursor;
}