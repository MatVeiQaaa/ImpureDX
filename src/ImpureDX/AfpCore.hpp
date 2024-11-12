#pragma once

#include <windows.h>

namespace afpcore {
	HMODULE handle = GetModuleHandle("afp-core.dll");

	typedef int(__fastcall* createLayerFromStream)(unsigned int streamId, const char* streamName);
	createLayerFromStream CreateLayerFromStream = (createLayerFromStream)GetProcAddress(handle, "XCd229cc00001e");

	typedef int(__fastcall* createLayerFromBitmap)(const char* resName, void* somePtrFor0x208);
	createLayerFromBitmap CreateLayerFromBitmap = (createLayerFromBitmap)GetProcAddress(handle, "XCd229cc000021");

	typedef int(__fastcall* createLayerFromSprite)(void* spriteInfo, void* somePtrFor0x208);
	createLayerFromSprite CreateLayerFromSprite = (createLayerFromSprite)GetProcAddress(handle, "XCd229cc000022");

	typedef int(__fastcall* createLayer)(int unkIdx, void* somePtrFor0x208);
	createLayer CreateLayer = (createLayer)GetProcAddress(handle, "XCd229cc000029");

	typedef int(__fastcall* layerSetInfoFromBitmap)(unsigned int layerId, const char* resName);
	layerSetInfoFromBitmap LayerSetInfoFromBitmap = (layerSetInfoFromBitmap)GetProcAddress(handle, "XCd229cc000023");

	typedef int(__fastcall* layerSetInfoFromSprite)(unsigned int layerId, void* spriteInfo);
	layerSetInfoFromSprite LayerSetInfoFromSprite = (layerSetInfoFromSprite)GetProcAddress(handle, "XCd229cc000024");
	
	typedef int(__fastcall* setLayerPosition)(unsigned int layerId, float positions[2]);
	setLayerPosition SetLayerPosition = (setLayerPosition)GetProcAddress(handle, "XCd229cc00002e");

	typedef int(__vectorcall* updateClipping)(unsigned int layerId, double arg1, double arg2, double arg3, double arg4);
	updateClipping UpdateClipping = (updateClipping)GetProcAddress(handle, "XCd229cc000030");

	typedef int(__vectorcall* updateClipping2)(unsigned int layerId, double arg1, double arg2, double arg3, double arg4);
	updateClipping2 UpdateClipping2 = (updateClipping2)GetProcAddress(handle, "XCd229cc000031");

	typedef int(__fastcall* setLayerZ)(unsigned int layerId, unsigned short Z);
	setLayerZ SetLayerZ = (setLayerZ)GetProcAddress(handle, "XCd229cc000033");

	typedef int(__fastcall* setBlendMode)(unsigned int layerId, int dst, int src);
	setBlendMode SetBlendMode = (setBlendMode)GetProcAddress(handle, "XCd229cc000037");

	typedef int(__fastcall* getPlayworkParent)(unsigned int playworkId, unsigned int mode);
	getPlayworkParent GetPlayworkParent = (getPlayworkParent)GetProcAddress(handle, "XCd229cc000069");

	typedef int(__fastcall* setPlayworkOpt)(unsigned int playworkId, unsigned int optId, long long arg1, long long arg2);
	setPlayworkOpt SetPlayworkOpt = (setPlayworkOpt)GetProcAddress(handle, "XCd229cc000073");

	typedef int(__fastcall* getInfo)(unsigned int opId, uintptr_t somePtr, DWORD arg3, DWORD arg4);
	getInfo GetInfo = (getInfo)GetProcAddress(handle, "XCd229cc000086");

	short* layersMaxCount = (short*)((uintptr_t)handle + 0xE2100 + 0x30);
	uintptr_t** layerArray = (uintptr_t**)((uintptr_t)handle + 0xE2100 + 0x10);

	uintptr_t** playworkArray = (uintptr_t**)((uintptr_t)handle + 0xE2100 + 0xF98);
	short* playworkMaxCount = (short*)((uintptr_t)handle + 0xE2100 + 0xF98 + 0x8);
}

namespace afputils {
	HMODULE handle = GetModuleHandle("afp-utils.dll");

	typedef void(__fastcall* getTextureInfo)(void* buffer, unsigned int packageId, const char* graphicName);
	getTextureInfo GetTextureInfo = (getTextureInfo)GetProcAddress(handle, "XE592acd000046");

	typedef void(__fastcall* textureToSpriteInfo)(void* buffer, void* source);
	textureToSpriteInfo TextureToSpriteInfo = (textureToSpriteInfo)GetProcAddress(handle, "XE592acd00004c");

	short* packagesMaxCount = (short*)((uintptr_t)handle + 0x28608);
	uintptr_t** packageArray = (uintptr_t**)((uintptr_t)handle + 0x28608 + 0x10);
}