#include "BaseModels/ModFeature.hpp"

#include <stdint.h>

class Unrandomizer : public ModFeature {
public:
	bool Init(uintptr_t moduleBase);
	bool Deinit();

	bool GetEnabled();
	void SetEnabled(bool value);
	void ToggleEnabled();

	bool GetBWPermute();
	void SetBWPermute(bool value);
	void ToggleBWPermute();

	void Menu();
private:
	enum UnrandomizerState {
		UnrandomizerState_P1,
		UnrandomizerState_P2,
		UnrandomizerState_DP
	};
	static void __fastcall OnShuffleColumns(DWORD* pThis, DWORD edx);
	void DragAndDropKeyDisplay(UnrandomizerState state);
	bool mIsEnabled = false;
	bool mIsBWPermute = false;
	uint32_t mLaneOrderL[7] = { 0, 1, 2, 3, 4, 5, 6 };
	uint32_t mLaneOrderR[7] = { 0, 1, 2, 3, 4, 5, 6 };
};