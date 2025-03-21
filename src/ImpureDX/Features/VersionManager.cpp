#include "VersionManager.hpp"
#include <cstdint>

#include <psapi.h>

#include "sigmatch/include/sigmatch/sigmatch.hpp"

uintptr_t ResolveRelativeOffset(uint32_t* offset) {
	uintptr_t offsetFrom = (uintptr_t)offset + sizeof(uint32_t);
	return offsetFrom + *offset;
}

bool VersionManager::Init(uintptr_t moduleBase) {
	VersionManager::mModuleBase = moduleBase;
	char moduleName[CHAR_MAX];
	GetModuleBaseName(GetCurrentProcess(), (HMODULE)moduleBase, moduleName, CHAR_MAX - 1);

	using namespace sigmatch_literals;
	sigmatch::this_process_target target;
	sigmatch::search_context context = target.in_module(moduleName);

	uintptr_t shuffleColumnsPatternOffset = (uintptr_t)context.search("BD E8 00 00 00 0F 1F 44 00 00 39 1D ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 74 07"_sig).matches().front();
	mFunctionPointers["ShuffleColumns"] = shuffleColumnsPatternOffset ? shuffleColumnsPatternOffset - 0x46 : 0;

	if (shuffleColumnsPatternOffset) {
		uintptr_t isDPOffset = ResolveRelativeOffset((uint32_t*)(shuffleColumnsPatternOffset + 0xC));
		mMemberPointers["State"] = isDPOffset - 4;

		uintptr_t optionsOffset = ResolveRelativeOffset((uint32_t*)(shuffleColumnsPatternOffset + 0x13));
		mMemberPointers["GeneralOptions"] = optionsOffset;
	}

	return true;
}

bool VersionManager::Deinit() {
	return true;
}

uintptr_t VersionManager::GetFunctionPointer(const char* name) {
	return mFunctionPointers[name];
}
uintptr_t VersionManager::GetMemberPointer(const char* name) {
	return mMemberPointers[name];
}