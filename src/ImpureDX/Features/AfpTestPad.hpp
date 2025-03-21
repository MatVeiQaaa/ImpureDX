#include "BaseModels/ModFeature.hpp"

class FAfpTestPad : public ModFeature {
public:
	bool Init(uintptr_t moduleBase);
	bool Deinit();

	void HideAllSpriteLayers();
	void SetAllLayersPos();
	void TestDump(unsigned int idx);
	void TestRenderSprite(unsigned int layerId, unsigned int packageId, const char* textureName);
	void TestRenderMovie(unsigned int layerId, unsigned int packageId, const char* movieName);
private:
	HMODULE m_hAfpCore = NULL;
};