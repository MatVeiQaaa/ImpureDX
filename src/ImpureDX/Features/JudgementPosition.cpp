#include "JudgementPosition.hpp"
#include "AfpCore.hpp"
#include <iostream>

//namespace bm2d {
//	HMODULE handle = GetModuleHandle("bm2dx.exe");
//
//	typedef int(__fastcall* drawSprite)(uintptr_t pThis, const char* name, );
//	changeSpriteFromInfo ChangeSpriteFromInfo = (changeSpriteFromInfo)GetProcAddress(handle, "XCd229cc00002e");
//}

bool FAfpTestPad::Init(uintptr_t moduleBase) {
	FAfpTestPad::mModuleBase = moduleBase;
	FAfpTestPad::m_hAfpCore = afpcore::handle;
	return true;
}

bool FAfpTestPad::Deinit() {
	return true;
}

void FAfpTestPad::HideAllSpriteLayers() {
	for (int i = 0; i < *afpcore::layersMaxCount; i++) {
		unsigned int* layerId = (unsigned int*)((*afpcore::layerArray)[i] + 0x18);
		if ((int)layerId != 0x18) afpcore::LayerSetAttribute(*layerId, 1, 0);
	}
}

void FAfpTestPad::SetAllLayersPos() {
	float position[2] = { 100.f, 100.f };
	for (int i = 0; i < *afpcore::layersMaxCount; i++) {
		unsigned int* layerId = (unsigned int*)((*afpcore::layerArray)[i] + 0x18);
		if ((int)layerId != 0x18) afpcore::LayerSetPosition(*layerId, position);
	}
}

void FAfpTestPad::TestDump(unsigned int idx) {
	afpcore::GetInfo(idx, 0x78000000, 0x2000, 0x2000);
}

void FAfpTestPad::TestRenderSprite(unsigned int layerIdIn, unsigned int packageId, const char* textureName) {
	void* buffer[sizeof(int) * 32];
	void* buffer2[sizeof(int) * 32];
	if (packageId) {
		afputils::GetBitmapInfoFromPackage(buffer, packageId, textureName);
	}
	else {
		afputils::GetBitmapInfo(buffer, textureName);
	}
	afputils::BitmapToSpriteInfo(buffer2, buffer);

	unsigned int layerId = layerIdIn;
	if (layerId) {
		afpcore::LayerSetInfoFromSprite(layerIdIn, buffer2);
	}
	else {
		layerId = afpcore::CreateLayerFromSprite(buffer2, 0);
	}

	float position[2] = { 100.f, 100.f };
	afpcore::LayerSetZ(layerId, 1);
	afpcore::LayerSetAttribute(layerId, 1, 1); // Visibility true.
	afpcore::LayerSetPosition(layerId, position);

	//for (int i = 0; i < *afpcore::layersMaxCount; i++) {
	//	unsigned int inLayerId = layerId;
	//	unsigned int* layerId = (unsigned int*)((*afpcore::layerArray)[i] + 0x18);
	//	if ((int)layerId != 0x18 && *layerId == inLayerId) {
	//		unsigned int* playworkId = (unsigned int*)((*afpcore::layerArray)[i] + 0x210);
	//		for (unsigned int parentPlayworkId = *playworkId; (parentPlayworkId & 0x80000000) == 0; parentPlayworkId = afpcore::GetPlayworkParent(parentPlayworkId, 6)) {
	//			afpcore::SetPlayworkOpt(parentPlayworkId, 0x1006, 13, 0); // blend mode
	//			afpcore::SetPlayworkOpt(parentPlayworkId, 0x101E, 1, 0);
	//		}
	//	}
	//}
}

void FAfpTestPad::TestRenderMovie(unsigned int layerIdIn, unsigned int packageId, const char* movieName) {
	afputils::MovieInfo movieInfo;

	if (packageId) {
		afputils::GetMovieInfoFromPackage(&movieInfo, packageId, movieName);
	}
	else {
		afputils::GetMovieInfo(&movieInfo, movieName);
	}

	unsigned int layerId = afpcore::CreateLayerFromStream(movieInfo.streamId, movieInfo.name, 0, 0);

	if (afpcore::AfpIdIsValid(5, layerId) < 0) return;

	afpcore::LayerMovieRefer(layerId, "/");
	afpcore::LayerSetAttribute(layerId, 0x200, 0x200); // Stop after one loop.
	afpcore::LayerSetAttribute(layerId, 1, 1); // Start the loop.
	afpcore::LayerSetZ(layerId, 200);
}