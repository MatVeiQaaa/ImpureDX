#include "BaseModels/ModBody.hpp"
#include "BaseModels/ImGuiMenu.hpp"
#include "BaseModels/ModFeature.hpp"

class AfpTestPadMenu : public ImGuiMenu {
public:
	void Loop();
	void MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	int getInfoIdx = 0;
	int layerIdx = 0;
	int packageIdx = 0;
	char textureName[MAXCHAR];
	char movieName[MAXCHAR];
};

class ImpureDXMenu : public ImGuiMenu {
public:
	void Loop();
	void MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
};

class ImpureDX : public ModBody {
public:

	static ImpureDX& Get();
	bool Hook();
	bool Unhook();

	ImpureDXMenu mMenu;
	AfpTestPadMenu mAfpTestPadMenu;

	ModFeature* mVersionManager;
	ModFeature* mFAfpTestPad;
	ModFeature* mGameState;
	ModFeature* mUnrandomizer;
private:
	ImpureDX() = default;
	~ImpureDX() = default;
	ImpureDX(const ImpureDX&) = delete;
	ImpureDX& operator=(const ImpureDX&) = delete;
};