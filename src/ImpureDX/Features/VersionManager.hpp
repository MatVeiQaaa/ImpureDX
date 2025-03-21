#include "BaseModels/ModFeature.hpp"

#include <unordered_map>

class VersionManager : public ModFeature {
public:
	bool Init(uintptr_t moduleBase);
	bool Deinit();

	uintptr_t GetFunctionPointer(const char* name);
	uintptr_t GetMemberPointer(const char* name);
private:
	std::unordered_map<const char*, uintptr_t> mFunctionPointers;
	std::unordered_map<const char*, uintptr_t> mMemberPointers;
};