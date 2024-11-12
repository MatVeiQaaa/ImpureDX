#include "JudgementPosition.hpp"
#include "AfpCore.hpp"
#include <iostream>

//namespace bm2d {
//	HMODULE handle = GetModuleHandle("bm2dx.exe");
//
//	typedef int(__fastcall* drawSprite)(uintptr_t pThis, const char* name, );
//	changeSpriteFromInfo ChangeSpriteFromInfo = (changeSpriteFromInfo)GetProcAddress(handle, "XCd229cc00002e");
//}

bool FJudgementPosition::Init(uintptr_t moduleBase) {
	FJudgementPosition::mModuleBase = moduleBase;
	FJudgementPosition::m_hAfpCore = afpcore::handle;
	return true;
}

bool FJudgementPosition::Deinit() {
	return true;
}

void FJudgementPosition::HideAllSpriteLayers() {
	for (int i = 0; i < *afpcore::layersMaxCount; i++) {
		unsigned int* layerType = (unsigned int*)((*afpcore::layerArray)[i] + 0x0);
		unsigned int* streamId = (unsigned int*)((*afpcore::layerArray)[i] + 0x4);
		if ((int)layerType != 0x0 && (*streamId != 0x70000005)) *layerType = 0;
	}
}

void FJudgementPosition::SetAllLayersPos() {
	float position[2] = { 100.f, 100.f };
	for (int i = 0; i < *afpcore::layersMaxCount; i++) {
		unsigned int* layerId = (unsigned int*)((*afpcore::layerArray)[i] + 0x18);
		if ((int)layerId != 0x18) afpcore::SetLayerPosition(*layerId, position);
	}
}

void FJudgementPosition::TestDump(unsigned int idx) {
	afpcore::GetInfo(idx, 0x78000000, 0x2000, 0x2000);
}

void FJudgementPosition::TestRender(unsigned int layerIdIn, unsigned int packageId, const char* textureName) {
	void* buffer[sizeof(int) * 32];
	void* buffer2[sizeof(int) * 32];
	afputils::GetTextureInfo(buffer, packageId, textureName);
	afputils::TextureToSpriteInfo(buffer2, buffer);
	//afpcore::LayerSetInfoFromSprite(layerId, buffer2); // theoretically can render to existing layers, but would require hooking afp::render, as game code constantly sets the original sprite back.

	unsigned int layerId = afpcore::CreateLayerFromSprite(buffer2, 0);

	float position[2] = { 100.f, 100.f };
	afpcore::SetLayerZ(layerId, 200);
	afpcore::SetBlendMode(layerId, 1, 1);
	afpcore::SetLayerPosition(layerId, position);


	for (int i = 0; i < *afpcore::layersMaxCount; i++) {
		unsigned int inLayerId = layerId;
		unsigned int* layerId = (unsigned int*)((*afpcore::layerArray)[i] + 0x18);
		if ((int)layerId != 0x18 && *layerId == inLayerId) {
			unsigned int* playworkId = (unsigned int*)((*afpcore::layerArray)[i] + 0x210);
			for (unsigned int parentPlayworkId = *playworkId; (parentPlayworkId & 0x80000000) == 0; parentPlayworkId = afpcore::GetPlayworkParent(parentPlayworkId, 6)) {
				afpcore::SetPlayworkOpt(parentPlayworkId, 0x1006, 13, 0); // blend mode
				afpcore::SetPlayworkOpt(parentPlayworkId, 0x101E, 1, 0);
			}
		}
	}
}