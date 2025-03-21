#include "Unrandomizer.hpp"

#include <iostream>
#include <string>
#include <array>
#include <random>
#include <chrono>
#include <algorithm>

#include "ImpureDX/ImpureDX.hpp"
#include "Features/GameState.hpp"
#include "Features/VersionManager.hpp"

#include "minhook/include/MinHook.h"
#include "imgui/imgui.h"

template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

typedef void(__thiscall* tShuffleColumns)(DWORD* pThis);
tShuffleColumns ShuffleColumns = nullptr;

void __fastcall Unrandomizer::OnShuffleColumns(DWORD* pThis, DWORD edx) {
	ShuffleColumns(pThis);
	Unrandomizer& unrandomizer = *(Unrandomizer*)(ImpureDX::Get().mUnrandomizer);
	if (!unrandomizer.GetEnabled()) return;
	GameState& gameState = *(GameState*)(ImpureDX::Get().mGameState);
	const GameState::GeneralOptions& options = gameState.GetOptions();
	const GameState::State& state = gameState.GetState();

	bool isDP = state.isDP;
	bool isP2 = state.isP2;

	if (isDP) {
		if (!(options.optionsDP.randomModeL == 1 || options.optionsDP.randomModeR == 1)) return;
	}
	else if (!isP2) {
		if (options.optionsP1.randomModeL != 1) return;
	}
	else if (options.optionsP2.randomModeL != 1) return;

	uint32_t* columnMappingArrayL = (uint32_t*)((size_t)pThis + 0x10);
	uint32_t* columnMappingArrayR = (uint32_t*)((size_t)columnMappingArrayL + sizeof(uint32_t) * 8);

	uint32_t* inputPositionArrayL;
	uint32_t* inputPositionArrayR;

	inputPositionArrayL = unrandomizer.mLaneOrderL;
	inputPositionArrayR = unrandomizer.mLaneOrderR;
	if (unrandomizer.GetBWPermute()) {
		uint32_t** inputArrays[2] = { &inputPositionArrayL, &inputPositionArrayR };
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		for (int side = 0; side < 2; side++) {
			std::vector<uint32_t> blueArray({ 1, 3, 5 });
			std::vector<uint32_t> whiteArray({ 0, 2, 4, 6 });
			uint32_t* resultingArray = (uint32_t*)alloca(sizeof(uint32_t) * 7);
			uint32_t* inputPositionArray = *inputArrays[side];
			std::shuffle(blueArray.begin(), blueArray.end(), std::default_random_engine(seed));
			std::shuffle(whiteArray.begin(), whiteArray.end(), std::default_random_engine(seed));

			for (int i = 0; i < std::size(unrandomizer.mLaneOrderL); i++) {
				uint32_t columnVal;
				if (inputPositionArray[i] % 2 != 0) {
					columnVal = *blueArray.begin();
					blueArray.erase(blueArray.begin());
				}
				else {
					columnVal = *whiteArray.begin();
					whiteArray.erase(whiteArray.begin());
				}
				resultingArray[i] = columnVal;
			}
			*inputArrays[side] = resultingArray;
		}
	}

	int32_t inputColumnMappingL[7];
	int32_t inputColumnMappingR[7];
	for (int i = 0; i < std::size(inputColumnMappingL); i++) {
		inputColumnMappingL[inputPositionArrayL[i]] = i;
		inputColumnMappingR[inputPositionArrayR[i]] = i;
	}

	if (isDP) {
		if (options.optionsDP.randomModeL == 1)
			memcpy(columnMappingArrayL, inputColumnMappingL, sizeof(inputColumnMappingL));
		if (options.optionsDP.randomModeR == 1)
			memcpy(columnMappingArrayR, inputColumnMappingR, sizeof(inputColumnMappingR));
	}
	else if (!isP2) {
		memcpy(columnMappingArrayL, inputColumnMappingL, sizeof(inputColumnMappingL));
	}
	else {
		memcpy(columnMappingArrayR, inputColumnMappingR, sizeof(inputColumnMappingR));
	}
}

bool Unrandomizer::Init(uintptr_t moduleBase) {
	Unrandomizer::mModuleBase = moduleBase;

	VersionManager& versionManager = *(VersionManager*)(ImpureDX::Get().mVersionManager);
	ShuffleColumns = (tShuffleColumns)(versionManager.GetFunctionPointer("ShuffleColumns"));
	if (!ShuffleColumns) return false;

	if (MH_CreateHookEx((LPVOID)ShuffleColumns, &OnShuffleColumns, &ShuffleColumns) != MH_OK) return false;

	if (MH_QueueEnableHook(MH_ALL_HOOKS) || MH_ApplyQueued() != MH_OK) return false;

	return true;
}

bool Unrandomizer::Deinit() {
	return true;
}

bool Unrandomizer::GetEnabled() {
	return mIsEnabled;
}

void Unrandomizer::SetEnabled(bool value) {
	mIsEnabled = value;
}

void Unrandomizer::ToggleEnabled() {
	mIsEnabled = !mIsEnabled;
}

bool Unrandomizer::GetBWPermute() {
	return mIsBWPermute;
}

void Unrandomizer::SetBWPermute(bool value) {
	mIsBWPermute = value;
}

void Unrandomizer::ToggleBWPermute() {
	mIsBWPermute = !mIsBWPermute;
}

void HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Unrandomizer::DragAndDropKeyDisplay(UnrandomizerState state) {
	ImGui::Text("Random Select");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol(ImGuiCol_Text), IM_COL32(196, 196, 196, 255));
	ImGui::Text("(drag and drop to reorder lanes)");
	ImGui::PopStyleColor();
	int sideCount = state == UnrandomizerState_DP ? 2 : 1;
	for (int side = 0; side < sideCount; side++) {
		uint32_t* laneOrder;
		std::string dragDropRefName;
		if (state == UnrandomizerState_DP)
		{
			if (side == 0) {
				laneOrder = mLaneOrderL;
				dragDropRefName = "RT_LANE_MEMBER_L";
			}
			else {
				laneOrder = mLaneOrderR;
				dragDropRefName = "RT_LANE_MEMBER_R";
			}
		}
		else {
			if (state == UnrandomizerState_P1) {
				laneOrder = mLaneOrderL;
				dragDropRefName = "RT_LANE_MEMBER_L";
			}
			else {
				laneOrder = mLaneOrderR;
				dragDropRefName = "RT_LANE_MEMBER_R";
			}
		}
		ImGui::NewLine();
		for (int i = 0; i < std::size(mLaneOrderL); i++) {
			ImGui::PushID(i + std::size(mLaneOrderL) * side);
			ImGui::SameLine();
			if (laneOrder[i] % 2 != 0) {
				ImGui::PushStyleColor(ImGuiCol(ImGuiCol_Button), IM_COL32(0, 0, 139, 255));
				ImGui::PushStyleColor(ImGuiCol(ImGuiCol_Text), IM_COL32(230, 230, 230, 255));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol(ImGuiCol_Button), IM_COL32(230, 230, 230, 255));
				ImGui::PushStyleColor(ImGuiCol(ImGuiCol_Text), IM_COL32(49, 49, 49, 255));
			}
			if (mIsBWPermute) {
				ImGui::Button("", ImVec2(50, 80));
			}
			else {
				ImGui::Button(std::to_string(laneOrder[i] + 1).c_str(), ImVec2(50, 80));
			}

			ImGui::PopStyleColor(2);

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags(ImGuiDragDropFlags_None))) {
				
				ImGui::SetDragDropPayload(dragDropRefName.c_str(), &i, sizeof(int));
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget()) {
				if (ImGui::AcceptDragDropPayload(dragDropRefName.c_str(), ImGuiDragDropFlags_None) != nullptr) {
					int payload_i = *(int*)ImGui::AcceptDragDropPayload(dragDropRefName.c_str())->Data;
					std::swap(laneOrder[i], laneOrder[payload_i]);
				}

				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();
		}
	}
}

void Unrandomizer::Menu() {
	GameState& gameState = *(GameState*)(ImpureDX::Get().mGameState);
	const GameState::State& state = gameState.GetState();
	bool sideDecided = state.isP1 | state.isP2;
	if (!sideDecided) {
		ImGui::Text("Waiting for side selection...");
		return;
	}
	UnrandomizerState unrandomizerState;
	if (!state.isDP) {
		unrandomizerState = state.isP1 ? UnrandomizerState_P1 : UnrandomizerState_P2;
	}
	else {
		unrandomizerState = UnrandomizerState_DP;
	}
	DragAndDropKeyDisplay(unrandomizerState);

	ImGui::Text("Controls");

	ImGui::Indent();
	ImGui::Checkbox("Trainer Enabled", &mIsEnabled);
	ImGui::SameLine();
	HelpMarker("When enabled the RANDOM play option will produce the selected random until disabled.\n\nThe selected random can be changed and the trainer toggled on or off between quick retries without needing to return to song select");
	//ImGui::Checkbox("Track Current Random", TRACK_RAN_WHEN_DISABLED);
	ImGui::SameLine();
	HelpMarker("While the trainer is disabled this option will update the key display to reflect the current random");
	ImGui::Checkbox("Black/White Random Select", &mIsBWPermute);
	ImGui::Unindent();
}