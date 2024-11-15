#pragma once

#include <windows.h>

namespace afpcore {
	HMODULE handle = GetModuleHandle("afp-core.dll");

	typedef int(__fastcall* afpIdIsValid)(unsigned int type, unsigned int layerId);
	afpIdIsValid AfpIdIsValid = (afpIdIsValid)GetProcAddress(handle, "XCd229cc000015");

	typedef int(__fastcall* createLayerFromStream)(unsigned int streamId, const char* streamName, long long unk1, long long unk2);
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
	
	typedef int(__fastcall* layerSetPosition)(unsigned int layerId, float positions[2]);
	layerSetPosition LayerSetPosition = (layerSetPosition)GetProcAddress(handle, "XCd229cc00002e");

	typedef int(__vectorcall* updateClipping)(unsigned int layerId, double arg1, double arg2, double arg3, double arg4);
	updateClipping UpdateClipping = (updateClipping)GetProcAddress(handle, "XCd229cc000030");

	typedef int(__vectorcall* updateClipping2)(unsigned int layerId, double arg1, double arg2, double arg3, double arg4);
	updateClipping2 UpdateClipping2 = (updateClipping2)GetProcAddress(handle, "XCd229cc000031");

	typedef int(__fastcall* layerSetZ)(unsigned int layerId, unsigned short Z);
	layerSetZ LayerSetZ = (layerSetZ)GetProcAddress(handle, "XCd229cc000033");

	typedef int(__fastcall* layerSetAttribute)(unsigned int layerId, unsigned int attribute, unsigned int value);
	layerSetAttribute LayerSetAttribute = (layerSetAttribute)GetProcAddress(handle, "XCd229cc000037");

	typedef int(__fastcall* layerMovieRefer)(unsigned int layerId, const char* path);
	layerMovieRefer LayerMovieRefer = (layerMovieRefer)GetProcAddress(handle, "XCd229cc000066");

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

	typedef void(__fastcall* getBitmapInfo)(void* buffer, const char* name);
	getBitmapInfo GetBitmapInfo = (getBitmapInfo)GetProcAddress(handle, "XE592acd000045");

	typedef void(__fastcall* getBitmapInfoFromPackage)(void* buffer, unsigned int packageId, const char* name);
	getBitmapInfoFromPackage GetBitmapInfoFromPackage = (getBitmapInfoFromPackage)GetProcAddress(handle, "XE592acd000046");

	typedef void(__fastcall* bitmapToSpriteInfo)(void* buffer, void* source);
	bitmapToSpriteInfo BitmapToSpriteInfo = (bitmapToSpriteInfo)GetProcAddress(handle, "XE592acd00004c");

	struct MovieInfo {
		long long unk = 0;
		long long afpId = 0;
		char* name = nullptr;
		long long streamId = 0;
	};

	typedef void(__fastcall* getMovieInfo)(MovieInfo* buffer, const char* name);
	getMovieInfo GetMovieInfo = (getMovieInfo)GetProcAddress(handle, "XE592acd000061");

	typedef void(__fastcall* getMovieInfoFromPackage)(MovieInfo* buffer, unsigned int packageId, const char* name);
	getMovieInfoFromPackage GetMovieInfoFromPackage = (getMovieInfoFromPackage)GetProcAddress(handle, "XE592acd000062");

	short* packagesMaxCount = (short*)((uintptr_t)handle + 0x28608);
	uintptr_t** packageArray = (uintptr_t**)((uintptr_t)handle + 0x28608 + 0x10);
}