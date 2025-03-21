#include "ImpureDX/ImpureDX.hpp"
#include "ImGuiInjector/ImGuiInjector.hpp"
#include "imgui/imgui.h"

#include "Features/AfpTestPad.hpp"
#include "Features/GameState.hpp"
#include "Features/Unrandomizer.hpp"
#include "Features/VersionManager.hpp"

#pragma comment(lib, "ImGuiInjector.lib")
#pragma comment(lib, "BaseModels.lib")

ImpureDX& ImpureDX::Get() {
	static ImpureDX instance;
	return instance;
}

bool ImpureDX::Hook() {
	ImGuiInjector::Get().AddMenu(&(ImpureDX::mMenu));
	ImGuiInjector::Get().AddMenu(&(ImpureDX::mAfpTestPadMenu));
	
	mModuleBase = (uintptr_t)GetModuleHandle(NULL);

	mVersionManager = new VersionManager();
	mFAfpTestPad = new FAfpTestPad();
	mGameState = new GameState();
	mUnrandomizer = new Unrandomizer();

	mVersionManager->Init(ImpureDX::mModuleBase);
	mFAfpTestPad->Init(ImpureDX::mModuleBase);
	mGameState->Init(ImpureDX::mModuleBase);
	mUnrandomizer->Init(ImpureDX::mModuleBase);

	return true;
}

bool ImpureDX::Unhook() {
	mVersionManager->Deinit();
	mFAfpTestPad->Deinit();
	mGameState->Deinit();
	mUnrandomizer->Deinit();
	delete(mVersionManager);
	delete(mFAfpTestPad);
	delete(mGameState);
	delete(mUnrandomizer);
	return true;
}

void ImpureDXMenu::Loop() {
	ImGui::Begin("ImpureDX", &(ImpureDXMenu::mIsOpen));
	ImGui::Text("ImpureDX Menu");

	if (ImGui::CollapsingHeader("Unrandomizer")) {
		((Unrandomizer*)ImpureDX::Get().mUnrandomizer)->Menu();
	}

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

int spriteSetArgs[2] = { 0, 0 };

void AfpTestPadMenu::Loop() {
	ImGui::Begin("AfpTestPad", &(AfpTestPadMenu::mIsOpen));
	ImGui::Text("Afp Test Pad Menu");
	FAfpTestPad& AfpTestPad = *(FAfpTestPad*)ImpureDX::Get().mFAfpTestPad;
	if (ImGui::Button("Hide All Sprite Layers")) AfpTestPad.HideAllSpriteLayers();
	if (ImGui::Button("Set All Layers Positions")) AfpTestPad.SetAllLayersPos();
	ImGui::InputInt("afpcore::GetInfo() Index", &getInfoIdx);
	if (ImGui::Button("Test Dump")) AfpTestPad.TestDump(getInfoIdx);
	ImGui::InputInt2("Layer ID and package ID for SpriteRender() and MovieRender()", spriteSetArgs);
	layerIdx = spriteSetArgs[0];
	packageIdx = spriteSetArgs[1];
	ImGui::InputText("Texture name for SpriteRender()", textureName, MAXCHAR);
	if (ImGui::Button("SpriteRender()")) AfpTestPad.TestRenderSprite(layerIdx, packageIdx, textureName);
	ImGui::InputText("Movie name for MovieRender()", movieName, MAXCHAR);
	if (ImGui::Button("MovieRender()")) AfpTestPad.TestRenderMovie(layerIdx, packageIdx, movieName);
	ImGui::End();
}

void AfpTestPadMenu::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (wParam) {
		/*case VK_INSERT: {
			AfpTestPadMenu::ToggleOpen();
			break;
		}*/
		}
		break;
	}
	}
}