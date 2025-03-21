#include "GameState.hpp"

#include "ImpureDX/ImpureDX.hpp"
#include "Features/VersionManager.hpp"

bool GameState::Init(uintptr_t moduleBase) {
	GameState::mModuleBase = moduleBase;

	VersionManager& versionManager = *(VersionManager*)(ImpureDX::Get().mVersionManager);
	options = (GeneralOptions**)(versionManager.GetMemberPointer("GeneralOptions"));
	if (!options) return false;
	state = (State*)(versionManager.GetMemberPointer("State"));
	if (!state) return false;

	return true;
}

bool GameState::Deinit() {
	return true;
}

const GameState::GeneralOptions& GameState::GetOptions() {
	return **options;
}

const GameState::State& GameState::GetState() {
	return *state;
}