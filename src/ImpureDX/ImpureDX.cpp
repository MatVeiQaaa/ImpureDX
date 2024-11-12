#include "ImpureDX/ImpureDX.hpp"
#include "ImGuiInjector/ImGuiInjector.hpp"
#include "imgui/imgui.h"

#pragma comment(lib, "ImGuiInjector.lib")
#pragma comment(lib, "BaseModels.lib")

ImpureDX& ImpureDX::Get() {
	static ImpureDX instance;
	return instance;
}

bool ImpureDX::Hook() {
	ImGuiInjector::Get().AddMenu(&(ImpureDX::mMenu));

	ImpureDX::mModuleBase = (uintptr_t)GetModuleHandle("bm2dx.exe");

	ImpureDX::mFJudgementPosition.Init(ImpureDX::mModuleBase);

	return true;
}

bool ImpureDX::Unhook() {
	return true;
}

int spriteSetArgs[2] = { 0, 0 };

void ImpureDXMenu::Loop() {
	ImGui::Begin("ImpureDX", &(ImpureDXMenu::mIsOpen));
	ImGui::Text("ImpureDX Menu");
	if (ImGui::Button("Hide All Sprite Layers")) ImpureDX::Get().mFJudgementPosition.HideAllSpriteLayers();
	if (ImGui::Button("Set All Layers Positions")) ImpureDX::Get().mFJudgementPosition.SetAllLayersPos();
	ImGui::InputInt("afpcore::GetInfo() Index", &getInfoIdx);
	if (ImGui::Button("Test Dump")) ImpureDX::Get().mFJudgementPosition.TestDump(getInfoIdx);
	ImGui::InputInt2("Input for Sprite Set", spriteSetArgs);
	layerIdx = spriteSetArgs[0];
	packageIdx = spriteSetArgs[1];
	ImGui::InputText("Texture name for Sprite Set", textureName, MAXCHAR);
	if (ImGui::Button("SetSprite()")) ImpureDX::Get().mFJudgementPosition.TestRender(layerIdx, packageIdx, textureName);
	ImGui::End();
}

void ImpureDXMenu::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_INSERT: {
			ImpureDXMenu::ToggleOpen();
			break;
		}
		}
		break;
	}
	}
}