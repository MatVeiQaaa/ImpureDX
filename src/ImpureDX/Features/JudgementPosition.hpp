#include "BaseModels/ModFeature.hpp"

class FJudgementPosition : public ModFeature {
public:
	bool Init(uintptr_t moduleBase);
	bool Deinit();

	void HideAllSpriteLayers();
	void SetAllLayersPos();
	void TestDump(unsigned int idx);
	void TestRender(unsigned int layerId, unsigned int packageId, const char* textureName);
private:
	HMODULE m_hAfpCore = NULL;
};